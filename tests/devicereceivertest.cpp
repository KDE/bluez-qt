/*****************************************************************************
 * This file is part of the QBluez project                                   *
 *                                                                           *
 * Copyright (C) 2010 Rafael Fernández López <ereslibre@kde.org>             *
 * Copyright (C) 2010 UFO Coders <info@ufocoders.com>                        *
 * Copyright (C) 2014 David Rosca <nowrep@gmail.com>                         *
 *                                                                           *
 * This library is free software; you can redistribute it and/or             *
 * modify it under the terms of the GNU Library General Public               *
 * License as published by the Free Software Foundation; either              *
 * version 2 of the License, or (at your option) any later version.          *
 *                                                                           *
 * This library is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         *
 * Library General Public License for more details.                          *
 *                                                                           *
 * You should have received a copy of the GNU Library General Public License *
 * along with this library; see the file COPYING.LIB.  If not, write to      *
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,      *
 * Boston, MA 02110-1301, USA.                                               *
 *****************************************************************************/

#include "devicereceivertest.h"

#include <QDebug>
#include <QTimer>
#include <QCoreApplication>

#include "manager.h"
#include "adapter.h"
#include "device.h"
#include "initmanagerjob.h"

using namespace QBluez;

Manager *g_manager = 0;

DeviceReceiver::DeviceReceiver(QObject *parent)
    : QObject(parent)
{
}

DeviceReceiver::~DeviceReceiver()
{
}

void DeviceReceiver::scanDevices()
{
    Adapter *usableAdapter = g_manager->usableAdapter();
    if (!usableAdapter && g_manager->adapters().isEmpty()) {
        qDebug() << "!!! No bluetooth adapters were found. Waiting for bluetooth adapters. Ctrl + C to cancel...";
        connect(g_manager, &Manager::adapterAdded, this, &DeviceReceiver::adapterAdded);
        return;
    } else if (!usableAdapter) {
        usableAdapter = g_manager->adapters().first();
        PendingCall *powerOnCall = usableAdapter->setPowered(true);
        powerOnCall->waitForFinished();
    }

    qDebug() << "*** Will scan devices until stopped...";
    qDebug();

    connect(usableAdapter, &Adapter::deviceFound, this, &DeviceReceiver::deviceFound);
    connect(usableAdapter, &Adapter::deviceChanged, this, &DeviceReceiver::devicePropertyChanged);

    usableAdapter->startDiscovery();
}

void DeviceReceiver::deviceFound(QBluez::Device *device)
{
    qDebug() << "*** Remote device found:" << device->name() << "(" << device->address() << ")";
    qDebug();
}

void DeviceReceiver::devicePropertyChanged(QBluez::Device *device)
{
    qDebug() << "*** Device with address" << device->address() << "changed some property";
    qDebug() << "\tAddress:\t" << device->address();
    qDebug() << "\tAlias:\t\t" << device->alias();
    qDebug() << "\tClass:\t\t" << device->deviceClass();
    qDebug() << "\tIcon:\t\t" << device->icon();
    qDebug() << "\tLegacy Pairing:\t" << (device->hasLegacyPairing() ? "yes" : "no");
    qDebug() << "\tName:\t\t" << device->name();
    qDebug() << "\tPaired:\t\t" << (device->isPaired() ? "yes" : "no");
    qDebug() << "\tTrusted:\t" << (device->isTrusted() ? "yes" : "no");
    qDebug() << "\tServices:\n" << device->uuids();
    qDebug();
}

void DeviceReceiver::adapterAdded(QBluez::Adapter *adapter)
{
    Q_UNUSED(adapter)

    qDebug() << "*** An adapter has been connected.";
    qDebug();
    scanDevices();
}

static void stopDiscovering()
{
    if (g_manager) {
        Q_FOREACH (Adapter *adapter, g_manager->adapters()) {
            adapter->stopDiscovery();
        }
    }
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    qAddPostRoutine(stopDiscovering);

    g_manager = new Manager();
    InitManagerJob *initJob = g_manager->init(Manager::InitManagerAndAdapters);
    initJob->exec();

    if (initJob->error()) {
        qWarning() << "Error initializing manager:" << initJob->errorText();
        return 1;
    }

    DeviceReceiver *deviceReceiver = new DeviceReceiver();
    deviceReceiver->scanDevices();

    return app.exec();
}
