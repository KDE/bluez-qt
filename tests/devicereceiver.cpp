/*
 * Copyright (C) 2010 Rafael Fernández López <ereslibre@kde.org>
 * Copyright (C) 2010 UFO Coders <info@ufocoders.com>
 * Copyright (C) 2014-2015 David Rosca <nowrep@gmail.com>
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

#include "devicereceiver.h"

#include <QDebug>
#include <QTimer>
#include <QCoreApplication>

#include "adapter.h"
#include "device.h"
#include "pendingcall.h"
#include "initmanagerjob.h"

using namespace BluezQt;

Manager *g_manager = Q_NULLPTR;

DeviceReceiver::DeviceReceiver(QObject *parent)
    : QObject(parent)
{
}

void DeviceReceiver::scanDevices()
{
    AdapterPtr usableAdapter = g_manager->usableAdapter();
    if (!usableAdapter && g_manager->adapters().isEmpty()) {
        qDebug() << "!!! No bluetooth adapters were found. Waiting for bluetooth adapters. Ctrl + C to cancel...";
        connect(g_manager, &Manager::adapterAdded, this, &DeviceReceiver::adapterAdded);
        return;
    } else if (!usableAdapter) {
        usableAdapter = g_manager->adapters().at(0);
        PendingCall *powerOnCall = usableAdapter->setPowered(true);
        powerOnCall->waitForFinished();
    }

    qDebug() << "*** Will scan devices until stopped...";
    qDebug();

    connect(usableAdapter.data(), &Adapter::deviceAdded, this, &DeviceReceiver::deviceFound);
    connect(usableAdapter.data(), &Adapter::deviceChanged, this, &DeviceReceiver::devicePropertyChanged);

    usableAdapter->startDiscovery();
}

void DeviceReceiver::deviceFound(const BluezQt::DevicePtr &device)
{
    qDebug() << "*** Remote device found:" << device->name() << "(" << device->address() << ")";
    qDebug();
}

void DeviceReceiver::devicePropertyChanged(const BluezQt::DevicePtr &device)
{
    qDebug() << "*** Device with address" << device->address() << "changed some property";
    qDebug() << "\tAddress:\t" << device->address();
    qDebug() << "\tClass:\t\t" << device->deviceClass();
    qDebug() << "\tIcon:\t\t" << device->icon();
    qDebug() << "\tLegacy Pairing:\t" << (device->hasLegacyPairing() ? "yes" : "no");
    qDebug() << "\tName:\t\t" << device->name();
    qDebug() << "\tPaired:\t\t" << (device->isPaired() ? "yes" : "no");
    qDebug() << "\tTrusted:\t" << (device->isTrusted() ? "yes" : "no");
    qDebug() << "\tServices:\n" << device->uuids();
    qDebug();
}

void DeviceReceiver::adapterAdded(const BluezQt::AdapterPtr &adapter)
{
    Q_UNUSED(adapter)

    qDebug() << "*** An adapter has been connected.";
    qDebug();
    scanDevices();
}

static void stopDiscovering()
{
    if (g_manager) {
        Q_FOREACH (const AdapterPtr &adapter, g_manager->adapters()) {
            adapter->stopDiscovery();
        }
    }
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    qAddPostRoutine(stopDiscovering);

    g_manager = new Manager();
    InitManagerJob *initJob = g_manager->init();
    initJob->exec();

    if (initJob->error()) {
        qWarning() << "Error initializing manager:" << initJob->errorText();
        return 1;
    }

    DeviceReceiver *deviceReceiver = new DeviceReceiver();
    deviceReceiver->scanDevices();

    return app.exec();
}
