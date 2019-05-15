/*
 * BluezQt - Asynchronous Bluez wrapper library
 *
 * Copyright (C) 2019 Manuel Weichselbaumer <mincequi@web.de>
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

#include "leadvertisingmanager.h"
#include "leadvertisingmanager_p.h"
#include "leadvertisement.h"
#include "leadvertisementadaptor.h"
#include "pendingcall.h"
#include "utils.h"
#include "debug.h"

namespace BluezQt
{

LEAdvertisingManager::LEAdvertisingManager(const QString &path, QObject *parent)
    : QObject(parent)
    , d(new LEAdvertisingManagerPrivate())
{
    d->m_path = path;
    d->m_bluezLEAdvertisingManager = new BluezLEAdvertisingManager(Strings::orgBluez(), path, DBusConnection::orgBluez(), this);
}

LEAdvertisingManager::~LEAdvertisingManager()
{
    delete d;
}

PendingCall *LEAdvertisingManager::registerAdvertisement(LEAdvertisement *advertisement)
{
    Q_ASSERT(advertisement);

    if (!d->m_bluezLEAdvertisingManager) {
        return new PendingCall(PendingCall::InternalError, QStringLiteral("LEAdvertisingManager not operational!"));
    }

    new LEAdvertisementAdaptor(advertisement);

    if (!DBusConnection::orgBluez().registerObject(advertisement->objectPath().path(), advertisement)) {
        qCDebug(BLUEZQT) << "Cannot register object" << advertisement->objectPath().path();
    }

    return new PendingCall(d->m_bluezLEAdvertisingManager->RegisterAdvertisement(advertisement->objectPath(), QVariantMap()),
                           PendingCall::ReturnVoid, this);
}

PendingCall *LEAdvertisingManager::unregisterAdvertisement(LEAdvertisement *advertisement)
{
    Q_ASSERT(advertisement);

    if (!d->m_bluezLEAdvertisingManager) {
        return new PendingCall(PendingCall::InternalError, QStringLiteral("LEAdvertisingManager not operational!"));
    }

    DBusConnection::orgBluez().unregisterObject(advertisement->objectPath().path());

    return new PendingCall(d->m_bluezLEAdvertisingManager->UnregisterAdvertisement(advertisement->objectPath()),
                           PendingCall::ReturnVoid, this);
}

} // namespace BluezQt
