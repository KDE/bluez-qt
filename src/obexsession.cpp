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

#include "obexsession.h"
#include "obexsession_p.h"
#include "pendingcall.h"
#include "utils.h"

namespace BluezQt
{

ObexSessionPrivate::ObexSessionPrivate(ObexSession *q, const QString &path)
    : QObject(q)
    , q(q)
{
    m_bluezSession = new BluezSession(Strings::orgBluezObex(),
                                      path, DBusConnection::orgBluezObex(), this);
}

void ObexSessionPrivate::init()
{
    DBusProperties dbusProperties(Strings::orgBluezObex(), m_bluezSession->path(),
                                  DBusConnection::orgBluezObex(), this);

    const QDBusPendingReply<QVariantMap> &call = dbusProperties.GetAll(Strings::orgBluezObexSession1());
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, &ObexSessionPrivate::getPropertiesFinished);
}

void ObexSessionPrivate::getPropertiesFinished(QDBusPendingCallWatcher *watcher)
{
    const QDBusPendingReply<QVariantMap> &reply = *watcher;
    watcher->deleteLater();

    if (reply.isError()) {
        Q_EMIT initError(reply.error().message());
        return;
    }

    const QVariantMap &properties = reply.value();

    m_source = properties.value(QStringLiteral("Source")).toString();
    m_destination = properties.value(QStringLiteral("Destination")).toString();
    m_channel = properties.value(QStringLiteral("Channel")).toUInt();
    m_target = properties.value(QStringLiteral("Target")).toString();
    m_root = properties.value(QStringLiteral("Root")).toString();

    Q_EMIT initFinished();
}

ObexSession::ObexSession(const QString &path, QObject *parent)
    : QObject(parent)
    , d(new ObexSessionPrivate(this, path))
{
}

ObexSession::~ObexSession()
{
    delete d;
}

QString ObexSession::source() const
{
    return d->m_source;
}

QString ObexSession::destination() const
{
    return d->m_destination;
}

quint8 ObexSession::channel() const
{
    return d->m_channel;
}

QString ObexSession::target() const
{
    return d->m_target;
}

QString ObexSession::root() const
{
    return d->m_root;
}

PendingCall *ObexSession::getCapabilities()
{
    return new PendingCall(d->m_bluezSession->GetCapabilities(), PendingCall::ReturnString, this);
}

} // namespace BluezQt
