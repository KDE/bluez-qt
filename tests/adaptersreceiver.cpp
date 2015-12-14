/*
 * Copyright (C) 2011 Rafael Fernández López <ereslibre@kde.org>
 * Copyright (C) 2011 UFO Coders <info@ufocoders.com>
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

#include "adaptersreceiver.h"

#include <QDebug>
#include <QCoreApplication>

#include "adapter.h"
#include "initmanagerjob.h"

using namespace BluezQt;

AdaptersReceiver::AdaptersReceiver(Manager *manager, QObject *parent)
    : QObject(parent)
    , m_manager(manager)
{
    connect(manager, &Manager::adapterAdded, this, &AdaptersReceiver::adapterAdded);
    connect(manager, &Manager::adapterRemoved, this, &AdaptersReceiver::adapterRemoved);
    connect(manager, &Manager::usableAdapterChanged, this, &AdaptersReceiver::usableAdapterChanged);
    connect(manager, &Manager::allAdaptersRemoved, this, &AdaptersReceiver::allAdaptersRemoved);
    connect(manager, &Manager::bluetoothOperationalChanged, this, &AdaptersReceiver::bluetoothOperationalChanged);
    connect(manager, &Manager::bluetoothBlockedChanged, this, &AdaptersReceiver::bluetoothBlockedChanged);
}

void AdaptersReceiver::adapterAdded(const BluezQt::AdapterPtr &adapter)
{
    qDebug() << "Adapter added: " << adapter->name();
    printStatus();
}

void AdaptersReceiver::adapterRemoved(const BluezQt::AdapterPtr &adapter)
{
    qDebug() << "Adapter removed: " << adapter->name();
    printStatus();
}

void AdaptersReceiver::usableAdapterChanged(const BluezQt::AdapterPtr &adapter)
{
    qDebug() << "Usable adapter changed: " << adapter;
    printStatus();
}

void AdaptersReceiver::allAdaptersRemoved()
{
    qDebug() << "All adapters have been removed";
    printStatus();
}

void AdaptersReceiver::bluetoothOperationalChanged(bool operational)
{
    qDebug() << "Bluetooth operational changed: " << operational;
    printStatus();
}

void AdaptersReceiver::bluetoothBlockedChanged(bool blocked)
{
    qDebug() << "Bluetooth blocked changed: " << blocked;
    printStatus();
}

void AdaptersReceiver::printStatus()
{
    qDebug() << "\tBluetooth Blocked: " << m_manager->isBluetoothBlocked();
    qDebug() << "\tBluetooth Operational: " << m_manager->isBluetoothOperational();
    qDebug() << "\tUsable Adapter: " << m_manager->usableAdapter();
    qDebug() << "";
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    qDebug() << "Looping until stopped";

    Manager *manager = new Manager();
    InitManagerJob *initJob = manager->init();
    initJob->exec();
    if (initJob->error()) {
        qWarning() << "Error initializing manager:" << initJob->errorText();
        return 1;
    }

    new AdaptersReceiver(manager);
    return app.exec();
}
