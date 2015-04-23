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

ObexSessionPrivate::ObexSessionPrivate(const QString &path, const QVariantMap &properties)
    : QObject()
{
    m_bluezSession = new BluezSession(Strings::orgBluezObex(),
                                      path, DBusConnection::orgBluezObex(), this);

    init(properties);
}

void ObexSessionPrivate::init(const QVariantMap &properties)
{
    m_source = properties.value(QStringLiteral("Source")).toString();
    m_destination = properties.value(QStringLiteral("Destination")).toString();
    m_channel = properties.value(QStringLiteral("Channel")).toUInt();
    m_target = properties.value(QStringLiteral("Target")).toString().toUpper();
    m_root = properties.value(QStringLiteral("Root")).toString();
}

ObexSession::ObexSession(const QString &path, const QVariantMap &properties)
    : QObject()
    , d(new ObexSessionPrivate(path, properties))
{
}

ObexSession::~ObexSession()
{
    delete d;
}

ObexSessionPtr ObexSession::toSharedPtr() const
{
    return d->q.toStrongRef();
}

QDBusObjectPath ObexSession::objectPath() const
{
    return QDBusObjectPath(d->m_bluezSession->path());
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
