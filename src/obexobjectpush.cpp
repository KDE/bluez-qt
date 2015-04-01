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

#include "obexobjectpush.h"
#include "pendingcall.h"
#include "utils.h"

#include "obexobjectpush1.h"

namespace BluezQt
{

typedef org::bluez::obex::ObjectPush1 BluezObjectPush;

class ObexObjectPushPrivate
{
public:
    ObexObjectPush *q;
    BluezObjectPush *m_bluezObjectPush;
};

ObexObjectPush::ObexObjectPush(const QDBusObjectPath &path, QObject *parent)
    : QObject(parent)
    , d(new ObexObjectPushPrivate)
{
    d->m_bluezObjectPush = new BluezObjectPush(Strings::orgBluezObex(),
                                               path.path(), DBusConnection::orgBluezObex(), this);
}

ObexObjectPush::~ObexObjectPush()
{
    delete d;
}

QDBusObjectPath ObexObjectPush::objectPath() const
{
    return QDBusObjectPath(d->m_bluezObjectPush->path());
}

PendingCall *ObexObjectPush::sendFile(const QString &fileName)
{
    return new PendingCall(d->m_bluezObjectPush->SendFile(fileName),
                           PendingCall::ReturnTransferWithProperties, this);
}

PendingCall *ObexObjectPush::pullBusinessCard(const QString &targetFileName)
{
    return new PendingCall(d->m_bluezObjectPush->PullBusinessCard(targetFileName),
                           PendingCall::ReturnTransferWithProperties, this);
}

PendingCall *ObexObjectPush::exchangeBusinessCards(const QString &clientFileName, const QString &targetFileName)
{
    return new PendingCall(d->m_bluezObjectPush->ExchangeBusinessCards(clientFileName, targetFileName),
                           PendingCall::ReturnTransferWithProperties, this);
}

} // namespace BluezQt
