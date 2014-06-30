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

#include "adaptertest.h"

#include <QDebug>
#include <QCoreApplication>

#include <QBluez/Manager>
#include <QBluez/Adapter>
#include <QBluez/LoadAdaptersJob>

using namespace QBluez;

AdapterTest::AdapterTest(Manager *manager, QObject *parent)
    : QThread(parent)
    , m_manager(manager)
{
}

AdapterTest::~AdapterTest()
{
}

void AdapterTest::adapterAdded(QBluez::Adapter *adapter)
{
    qDebug() << "Adapter added: " << adapter;
    qDebug() << "\tBluetooth Operational: " << m_manager->isBluetoothOperational();
    qDebug() << "\tUsable Adapter: " << m_manager->usableAdapter();
    qDebug();
}

void AdapterTest::adapterRemoved(QBluez::Adapter *adapter)
{
    qDebug() << "Adapter removed: " << adapter;
    qDebug() << "\tBluetooth Operational: " << m_manager->isBluetoothOperational();
    qDebug() << "\tUsable Adapter: " << m_manager->usableAdapter();
    qDebug();
}

void AdapterTest::usableAdapterChanged(QBluez::Adapter *adapter)
{
    qDebug() << "Usable adapter changed: " << adapter;
    qDebug() << "\tBluetooth Operational: " << m_manager->isBluetoothOperational();
    qDebug() << "\tUsable Adapter: " << m_manager->usableAdapter();
    qDebug();
}

void AdapterTest::allAdaptersRemoved()
{
    qDebug() << "All adapters have been removed";
    qDebug() << "\tBluetooth Operational: " << m_manager->isBluetoothOperational();
    qDebug() << "\tUsable Adapter: " << m_manager->usableAdapter();
    qDebug();
}

void AdapterTest::run()
{
    while (true) {
        qDebug();
        qDebug() << "Bluetooth Operational: " << m_manager->isBluetoothOperational();
        qDebug() << "Usable Adapter: " << m_manager->usableAdapter();
        qDebug();
        sleep(5);
    }
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    qDebug() << "Looping until stopped";

    GetManagerJob *managerJob = Manager::get();
    managerJob->exec();
    if (managerJob->error() != GetManagerJob::NoError) {
        qWarning() << "Error getting manager:" << managerJob->errorText();
        return 1;
    }

    Manager *manager = managerJob->manager();
    AdapterTest *adapterTest = new AdapterTest(manager);

    LoadAdaptersJob *adaptersJob = manager->loadAdapters();
    adaptersJob->exec();
    if (adaptersJob->error() != LoadAdaptersJob::NoError) {
        qWarning() << "Error loading adapters:" << adaptersJob->errorText();
        return 1;
    }

    QObject::connect(manager, &Manager::adapterAdded, adapterTest, &AdapterTest::adapterAdded);
    QObject::connect(manager, &Manager::adapterRemoved, adapterTest, &AdapterTest::adapterRemoved);
    QObject::connect(manager, &Manager::usableAdapterChanged, adapterTest, &AdapterTest::usableAdapterChanged);
    QObject::connect(manager, &Manager::allAdaptersRemoved, adapterTest, &AdapterTest::allAdaptersRemoved);

    adapterTest->start();

    return app.exec();
}
