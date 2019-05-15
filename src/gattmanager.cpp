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

#include "gattmanager.h"

#include "gattapplication.h"
#include "gattcharacteristic.h"
#include "gattcharacteristicadaptor.h"
#include "gattmanager_p.h"
#include "gattservice.h"
#include "gattserviceadaptor.h"
#include "objectmanageradaptor.h"
#include "pendingcall.h"
#include "utils.h"
#include "debug.h"

#include <QDBusInterface>
#include <QDBusPendingCall>

namespace BluezQt
{

GattManager::GattManager(const QString &path, QObject *parent)
    : QObject(parent)
    , d(new GattManagerPrivate(path))
{
}

GattManager::~GattManager()
{
    delete d;
}

PendingCall *GattManager::registerApplication(GattApplication *application)
{
    Q_ASSERT(application);

    const auto services = application->findChildren<GattService*>();
    for (auto service : services) {
        new GattServiceAdaptor(service);

        const auto charcs = service->findChildren<GattCharacteristic*>();
        for (auto charc : charcs) {
            new GattCharacteristicAdaptor(charc);
            if (!DBusConnection::orgBluez().registerObject(charc->objectPath().path(),
                                                           charc,
                                                           QDBusConnection::ExportAdaptors)) {
                qCDebug(BLUEZQT) << "Cannot register object" << charc->objectPath().path();
            }
        }

        if (!DBusConnection::orgBluez().registerObject(service->objectPath().path(),
                                                       service,
                                                       QDBusConnection::ExportAdaptors)) {
            qCDebug(BLUEZQT) << "Cannot register object" << service->objectPath().path();
        }
    }

    new ObjectManagerAdaptor(application);

    if (!DBusConnection::orgBluez().registerObject(application->objectPath().path(),
                                                   application,
                                                   QDBusConnection::ExportAdaptors)) {
        qCDebug(BLUEZQT) << "Cannot register object" << application->objectPath().path();
    }

    QList<QVariant> argumentList;
    argumentList << QVariant::fromValue(application->objectPath()) << QVariantMap();
    return new PendingCall(d->m_dbusInterface->asyncCallWithArgumentList(QStringLiteral("RegisterApplication"), argumentList),
                           PendingCall::ReturnVoid, this);
}

PendingCall *GattManager::unregisterApplication(GattApplication *application)
{
    Q_ASSERT(application);

    DBusConnection::orgBluez().unregisterObject(application->objectPath().path());

    QList<QVariant> argumentList;
    argumentList << QVariant::fromValue(application->objectPath());
    return new PendingCall(d->m_dbusInterface->asyncCallWithArgumentList(QStringLiteral("UnregisterApplication"), argumentList),
                           PendingCall::ReturnVoid, this);
}

} // namespace BluezQt
