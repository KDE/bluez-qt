/*
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

#include "leserver.h"

#include <QCoreApplication>
#include <QDebug>

#include "adapter.h"
#include "device.h"
#include "gattapplication.h"
#include "gattcharacteristic.h"
#include "gattmanager.h"
#include "gattservice.h"
#include "initmanagerjob.h"
#include "leadvertisement.h"
#include "leadvertisingmanager.h"
#include "manager.h"
#include "pendingcall.h"

using namespace BluezQt;

LeServer::LeServer(Manager *manager, QObject *parent)
    : QObject(parent),
      m_manager(manager)
{
    auto advertisement = new LEAdvertisement({QStringLiteral("ad100000-d901-11e8-9f8b-f2801f1b9fd1")}, this);
    auto call = m_manager->usableAdapter()->leAdvertisingManager()->registerAdvertisement(advertisement);
    connect(call, &PendingCall::finished, this, &LeServer::onCallFinished);

    auto application = new GattApplication(QStringLiteral("/org/kde/bluezqt"), this);
    auto service = new GattService(QStringLiteral("ad100000-d901-11e8-9f8b-f2801f1b9fd1"), true, application);
    new GattCharacteristic(QStringLiteral("ad10e100-d901-11e8-9f8b-f2801f1b9fd1"), service);
    auto call2 = m_manager->usableAdapter()->gattManager()->registerApplication(application);
    connect(call2, &PendingCall::finished, this, &LeServer::onCallFinished);
}

void LeServer::onCallFinished(BluezQt::PendingCall *call)
{
    if (call->error()) {
        qWarning() << "Error: " << call->errorText();
        return;
    }
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    qDebug() << "Advertising LE services. Ctrl + C to cancel...";

    Manager *manager = new Manager();
    InitManagerJob *initJob = manager->init();
    initJob->exec();
    if (initJob->error()) {
        qWarning() << "Error initializing manager:" << initJob->errorText();
        return 1;
    }

    if (!manager->usableAdapter()) {
        qWarning() << "No usable adapter";
        return 2;
    }
    if (!manager->usableAdapter()->leAdvertisingManager()) {
        qWarning() << "No LE advertising manager";
        return 2;
    }
    if (!manager->usableAdapter()->gattManager()) {
        qWarning() << "No GATT manager";
        return 2;
    }

    new LeServer(manager);

    return app.exec();
}
