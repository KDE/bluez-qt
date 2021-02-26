/*
 * BluezQt - Asynchronous Bluez wrapper library
 *
 * SPDX-FileCopyrightText: 2019 Manuel Weichselbaumer <mincequi@web.de>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "gattmanager.h"

#include "debug.h"
#include "gattapplication.h"
#include "gattcharacteristic.h"
#include "gattcharacteristicadaptor.h"
#include "gattmanager_p.h"
#include "gattservice.h"
#include "gattserviceadaptor.h"
#include "objectmanageradaptor.h"
#include "pendingcall.h"
#include "utils.h"

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

    const auto services = application->findChildren<GattService *>();
    for (auto service : services) {
        new GattServiceAdaptor(service);

        const auto charcs = service->findChildren<GattCharacteristic *>();
        for (auto charc : charcs) {
            new GattCharacteristicAdaptor(charc);
            if (!DBusConnection::orgBluez().registerObject(charc->objectPath().path(), charc, QDBusConnection::ExportAdaptors)) {
                qCDebug(BLUEZQT) << "Cannot register object" << charc->objectPath().path();
            }
        }

        if (!DBusConnection::orgBluez().registerObject(service->objectPath().path(), service, QDBusConnection::ExportAdaptors)) {
            qCDebug(BLUEZQT) << "Cannot register object" << service->objectPath().path();
        }
    }

    new ObjectManagerAdaptor(application);

    if (!DBusConnection::orgBluez().registerObject(application->objectPath().path(), application, QDBusConnection::ExportAdaptors)) {
        qCDebug(BLUEZQT) << "Cannot register object" << application->objectPath().path();
    }

    QList<QVariant> argumentList;
    argumentList << QVariant::fromValue(application->objectPath()) << QVariantMap();
    return new PendingCall(d->m_dbusInterface->asyncCallWithArgumentList(QStringLiteral("RegisterApplication"), argumentList), PendingCall::ReturnVoid, this);
}

PendingCall *GattManager::unregisterApplication(GattApplication *application)
{
    Q_ASSERT(application);

    DBusConnection::orgBluez().unregisterObject(application->objectPath().path());

    QList<QVariant> argumentList;
    argumentList << QVariant::fromValue(application->objectPath());
    return new PendingCall(d->m_dbusInterface->asyncCallWithArgumentList(QStringLiteral("UnregisterApplication"), argumentList), PendingCall::ReturnVoid, this);
}

} // namespace BluezQt
