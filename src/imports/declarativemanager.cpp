/*
 * QBluez - Asynchronous Bluez wrapper library
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

#include "declarativemanager.h"
#include "initmanagerjob.h"
#include "adapter.h"
#include "device.h"

DeclarativeManager *s_instance = 0;

static int adaptersCountFunction(QQmlListProperty<QBluez::Adapter> *property)
{
    Q_ASSERT(qobject_cast<DeclarativeManager*>(property->object));
    DeclarativeManager *manager = static_cast<DeclarativeManager*>(property->object);

    return manager->adapters().count();
}

static QBluez::Adapter *adaptersAtFunction(QQmlListProperty<QBluez::Adapter> *property, int index)
{
    Q_ASSERT(qobject_cast<DeclarativeManager*>(property->object));
    DeclarativeManager *manager = static_cast<DeclarativeManager*>(property->object);

    return manager->adapters().at(index);
}

static int devicesCountFunction(QQmlListProperty<QBluez::Device> *property)
{
    Q_ASSERT(qobject_cast<DeclarativeManager*>(property->object));
    DeclarativeManager *manager = static_cast<DeclarativeManager*>(property->object);

    return manager->devices().count();
}

static QBluez::Device *devicesAtFunction(QQmlListProperty<QBluez::Device> *property, int index)
{
    Q_ASSERT(qobject_cast<DeclarativeManager*>(property->object));
    DeclarativeManager *manager = static_cast<DeclarativeManager*>(property->object);

    return manager->devices().at(index);
}

DeclarativeManager::DeclarativeManager(QObject *parent)
    : QBluez::Manager(parent)
{
    if (s_instance) {
        qWarning("DeclarativeManager: Only one instance is allowed!");
    }
    s_instance = this;

    QBluez::InitManagerJob *job = init();
    job->start();
    connect(job, &QBluez::InitManagerJob::result, this, &DeclarativeManager::initJobResult);

    connect(this, &QBluez::Manager::adapterAdded, this, &DeclarativeManager::slotAdapterAdded);
    connect(this, &QBluez::Manager::adapterRemoved, this, &DeclarativeManager::slotAdapterRemoved);
}

QQmlListProperty<QBluez::Adapter> DeclarativeManager::declarativeAdapters()
{
    return QQmlListProperty<QBluez::Adapter>(this, 0, adaptersCountFunction, adaptersAtFunction);
}

QQmlListProperty<QBluez::Device> DeclarativeManager::declarativeDevices()
{
    return QQmlListProperty<QBluez::Device>(this, 0, devicesCountFunction, devicesAtFunction);
}

DeclarativeManager *DeclarativeManager::self()
{
    return s_instance;
}

void DeclarativeManager::initJobResult(QBluez::InitManagerJob *job)
{
    if (job->error()) {
        Q_EMIT initializeError(job->errorText());
        return;
    }

    Q_EMIT initialized();
}

void DeclarativeManager::slotAdapterAdded(QBluez::Adapter *adapter)
{
    Q_EMIT adaptersChanged(declarativeAdapters());

    connect(adapter, &QBluez::Adapter::deviceFound, this, &DeclarativeManager::slotDeviceAdded);
    connect(adapter, &QBluez::Adapter::deviceRemoved, this, &DeclarativeManager::slotDeviceRemoved);
}

void DeclarativeManager::slotAdapterRemoved(QBluez::Adapter *adapter)
{
    Q_EMIT adaptersChanged(declarativeAdapters());

    disconnect(adapter, &QBluez::Adapter::deviceFound, this, &DeclarativeManager::slotDeviceAdded);
    disconnect(adapter, &QBluez::Adapter::deviceRemoved, this, &DeclarativeManager::slotDeviceRemoved);
}

void DeclarativeManager::slotDeviceAdded(QBluez::Device *device)
{
    Q_UNUSED(device)
    Q_EMIT devicesChanged(declarativeDevices());
}

void DeclarativeManager::slotDeviceRemoved(QBluez::Device *device)
{
    Q_UNUSED(device)
    Q_EMIT devicesChanged(declarativeDevices());
}
