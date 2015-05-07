/*
 * BluezQt - Asynchronous Bluez wrapper library
 *
 * Copyright (C) 2014 David Rosca <nowrep@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "request.h"
#include "debug.h"
#include "utils.h"

#include <QStringBuilder>
#include <QDBusMessage>
#include <QDBusConnection>

namespace BluezQt
{

class RequestPrivate
{
public:
    RequestOriginatingType m_type;
    QDBusMessage m_message;

    bool sendMessage(const QDBusMessage &msg);
    QString interfaceName();

    void acceptRequest(const QVariant &val);
    void rejectRequest();
    void cancelRequest();
};

bool RequestPrivate::sendMessage(const QDBusMessage &msg)
{
    switch (m_type) {
    case OrgBluezAgent:
    case OrgBluezProfile:
        return DBusConnection::orgBluez().send(msg);
    case OrgBluezObexAgent:
        return DBusConnection::orgBluezObex().send(msg);

    default:
        return false;
    }
}

QString RequestPrivate::interfaceName()
{
    switch (m_type) {
    case OrgBluezAgent:
        return QStringLiteral("org.bluez.Agent1");
    case OrgBluezProfile:
        return QStringLiteral("org.bluez.Profile1");
    case OrgBluezObexAgent:
        return QStringLiteral("org.bluez.obex.Agent1");
    default:
        return QString();
    }
}

void RequestPrivate::acceptRequest(const QVariant &val)
{
    QDBusMessage reply;
    if (val.isValid()) {
        reply = m_message.createReply(val);
    } else {
        reply = m_message.createReply();
    }

    if (!sendMessage(reply)) {
        qCWarning(BLUEZQT) << "Request: Failed to put reply on DBus queue";
    }
}

void RequestPrivate::rejectRequest()
{
    const QDBusMessage &reply = m_message.createErrorReply(interfaceName() % QStringLiteral(".Rejected"),
                                QStringLiteral("Rejected"));
    if (!sendMessage(reply)) {
        qCWarning(BLUEZQT) << "Request: Failed to put reply on DBus queue";
    }
}

void RequestPrivate::cancelRequest()
{
    const QDBusMessage &reply = m_message.createErrorReply(interfaceName() % QStringLiteral(".Canceled"),
                                QStringLiteral("Canceled"));
    if (!sendMessage(reply)) {
        qCWarning(BLUEZQT) << "Request: Failed to put reply on DBus queue";
    }
}

// T
template<typename T>
Request<T>::Request()
    : d(new RequestPrivate)
{
}

template<typename T>
Request<T>::Request(RequestOriginatingType type, const QDBusMessage &message)
    : d(new RequestPrivate)
{
    d->m_type = type;
    d->m_message = message;
}

template<typename T>
Request<T>::~Request()
{
}

template<typename T>
Request<T>::Request(const Request &other)
    : d(other.d)
{
}

template<typename T>
Request<T> &Request<T>::operator=(const Request<T> &other)
{
    if (d != other.d) {
        d = other.d;
    }
    return *this;
}

template<typename T>
void Request<T>::accept(T returnValue) const
{
    d->acceptRequest(returnValue);
}

template<typename T>
void Request<T>::reject() const
{
    d->rejectRequest();
}

template<typename T>
void Request<T>::cancel() const
{
    d->cancelRequest();
}

// void
Request<void>::Request()
    : d(new RequestPrivate)
{
}

Request<void>::Request(RequestOriginatingType type, const QDBusMessage &message)
    : d(new RequestPrivate)
{
    d->m_type = type;
    d->m_message = message;
}

Request<void>::~Request()
{
}

Request<void>::Request(const Request &other)
    : d(other.d)
{
}

Request<void> &Request<void>::operator=(const Request<void> &other)
{
    if (d != other.d) {
        d = other.d;
    }
    return *this;
}

void Request<void>::accept() const
{
    d->acceptRequest(QVariant());
}

void Request<void>::reject() const
{
    d->rejectRequest();
}

void Request<void>::cancel() const
{
    d->cancelRequest();
}

// Generate classes
template class Request<void>;
template class Request<quint32>;
template class Request<QString>;

} // namespace BluezQt
