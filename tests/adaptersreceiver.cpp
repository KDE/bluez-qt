/*****************************************************************************
 * This file is part of the QBluez project                                   *
 *                                                                           *
 * Copyright (C) 2011 Rafael Fernández López <ereslibre@kde.org>             *
 * Copyright (C) 2011 UFO Coders <info@ufocoders.com>                        *
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

#include "adaptersreceiver.h"

#include <QDebug>
#include <QCoreApplication>

#include "manager.h"
#include "adapter.h"
#include "initmanagerjob.h"

using namespace QBluez;

AdaptersReceiver::AdaptersReceiver(Manager *manager, QObject *parent)
    : QObject(parent)
    , m_manager(manager)
{
    connect(manager, &Manager::adapterAdded, this, &AdaptersReceiver::adapterAdded);
    connect(manager, &Manager::adapterRemoved, this, &AdaptersReceiver::adapterRemoved);
    connect(manager, &Manager::usableAdapterChanged, this, &AdaptersReceiver::usableAdapterChanged);
    connect(manager, &Manager::allAdaptersRemoved, this, &AdaptersReceiver::allAdaptersRemoved);
    connect(manager, &Manager::bluetoothOperationalChanged, this, &AdaptersReceiver::bluetoothOperationalChanged);
}

void AdaptersReceiver::adapterAdded(QBluez::Adapter *adapter)
{
    qDebug() << "Adapter added: " << adapter;
    qDebug() << "\tBluetooth Operational: " << m_manager->isBluetoothOperational();
    qDebug() << "\tUsable Adapter: " << m_manager->usableAdapter();
    qDebug();
}

void AdaptersReceiver::adapterRemoved(QBluez::Adapter *adapter)
{
    qDebug() << "Adapter removed: " << adapter;
    qDebug() << "\tBluetooth Operational: " << m_manager->isBluetoothOperational();
    qDebug() << "\tUsable Adapter: " << m_manager->usableAdapter();
    qDebug();
}

void AdaptersReceiver::usableAdapterChanged(QBluez::Adapter *adapter)
{
    qDebug() << "Usable adapter changed: " << adapter;
    qDebug() << "\tBluetooth Operational: " << m_manager->isBluetoothOperational();
    qDebug() << "\tUsable Adapter: " << m_manager->usableAdapter();
    qDebug();
}

void AdaptersReceiver::allAdaptersRemoved()
{
    qDebug() << "All adapters have been removed";
    qDebug() << "\tBluetooth Operational: " << m_manager->isBluetoothOperational();
    qDebug() << "\tUsable Adapter: " << m_manager->usableAdapter();
    qDebug();
}

void AdaptersReceiver::bluetoothOperationalChanged(bool operational)
{
    qDebug() << "Bluetooth operational changed: " << operational;
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
