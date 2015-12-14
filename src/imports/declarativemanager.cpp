/*
 * BluezQt - Asynchronous Bluez wrapper library
 *
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

#include "declarativemanager.h"
#include "declarativeadapter.h"
#include "declarativedevice.h"
#include "initmanagerjob.h"
#include "adapter.h"
#include "device.h"

#include <QQmlEngine>

static int adaptersCountFunction(QQmlListProperty<DeclarativeAdapter> *property)
{
    Q_ASSERT(qobject_cast<DeclarativeManager*>(property->object));
    DeclarativeManager *manager = static_cast<DeclarativeManager*>(property->object);

    return manager->m_adapters.count();
}

static DeclarativeAdapter *adaptersAtFunction(QQmlListProperty<DeclarativeAdapter> *property, int index)
{
    Q_ASSERT(qobject_cast<DeclarativeManager*>(property->object));
    DeclarativeManager *manager = static_cast<DeclarativeManager*>(property->object);

    return manager->m_adapters.values().at(index);
}

static int devicesCountFunction(QQmlListProperty<DeclarativeDevice> *property)
{
    Q_ASSERT(qobject_cast<DeclarativeManager*>(property->object));
    DeclarativeManager *manager = static_cast<DeclarativeManager*>(property->object);

    return manager->m_devices.count();
}

static DeclarativeDevice *devicesAtFunction(QQmlListProperty<DeclarativeDevice> *property, int index)
{
    Q_ASSERT(qobject_cast<DeclarativeManager*>(property->object));
    DeclarativeManager *manager = static_cast<DeclarativeManager*>(property->object);

    return manager->m_devices.values().at(index);
}

DeclarativeManager::DeclarativeManager(QObject *parent)
    : BluezQt::Manager(parent)
{
    BluezQt::InitManagerJob *job = init();
    job->start();
    connect(job, &BluezQt::InitManagerJob::result, this, &DeclarativeManager::initJobResult);

    connect(this, &BluezQt::Manager::adapterAdded, this, &DeclarativeManager::slotAdapterAdded);
    connect(this, &BluezQt::Manager::adapterRemoved, this, &DeclarativeManager::slotAdapterRemoved);
    connect(this, &BluezQt::Manager::usableAdapterChanged, this, &DeclarativeManager::slotUsableAdapterChanged);
    connect(this, &BluezQt::Manager::deviceAdded, this, &DeclarativeManager::slotDeviceAdded);
    connect(this, &BluezQt::Manager::deviceRemoved, this, &DeclarativeManager::slotDeviceRemoved);

    connect(this, &BluezQt::Manager::adapterChanged, this, [this](const BluezQt::AdapterPtr &adapter) {
        Q_EMIT adapterChanged(declarativeAdapterFromPtr(adapter));
    });

    connect(this, &BluezQt::Manager::deviceChanged, this, [this](const BluezQt::DevicePtr &device) {
        Q_EMIT deviceChanged(declarativeDeviceFromPtr(device));
    });
}

DeclarativeAdapter *DeclarativeManager::usableAdapter() const
{
    return declarativeAdapterFromPtr(BluezQt::Manager::usableAdapter());
}

QQmlListProperty<DeclarativeAdapter> DeclarativeManager::declarativeAdapters()
{
    return QQmlListProperty<DeclarativeAdapter>(this, 0, adaptersCountFunction, adaptersAtFunction);
}

QQmlListProperty<DeclarativeDevice> DeclarativeManager::declarativeDevices()
{
    return QQmlListProperty<DeclarativeDevice>(this, 0, devicesCountFunction, devicesAtFunction);
}

DeclarativeAdapter *DeclarativeManager::declarativeAdapterFromPtr(BluezQt::AdapterPtr ptr) const
{
    if (!ptr) {
        return Q_NULLPTR;
    }
    return m_adapters.value(ptr->ubi());
}

DeclarativeDevice *DeclarativeManager::declarativeDeviceFromPtr(BluezQt::DevicePtr ptr) const
{
    if (!ptr) {
        return Q_NULLPTR;
    }
    return m_devices.value(ptr->ubi());
}

DeclarativeAdapter *DeclarativeManager::adapterForAddress(const QString &address) const
{
    return declarativeAdapterFromPtr(BluezQt::Manager::adapterForAddress(address));
}

DeclarativeAdapter *DeclarativeManager::adapterForUbi(const QString &ubi) const
{
    return declarativeAdapterFromPtr(BluezQt::Manager::adapterForUbi(ubi));
}

DeclarativeDevice *DeclarativeManager::deviceForAddress(const QString &address) const
{
    return declarativeDeviceFromPtr(BluezQt::Manager::deviceForAddress(address));
}

DeclarativeDevice *DeclarativeManager::deviceForUbi(const QString &ubi) const
{
    return declarativeDeviceFromPtr(BluezQt::Manager::deviceForUbi(ubi));
}

void DeclarativeManager::initJobResult(BluezQt::InitManagerJob *job)
{
    if (job->error()) {
        Q_EMIT initError(job->errorText());
        return;
    }

    Q_EMIT initFinished();
}

void DeclarativeManager::slotAdapterAdded(BluezQt::AdapterPtr adapter)
{
    DeclarativeAdapter *dAdapter = new DeclarativeAdapter(adapter, this);
    m_adapters[adapter->ubi()] = dAdapter;

    Q_EMIT adapterAdded(dAdapter);
    Q_EMIT adaptersChanged(declarativeAdapters());
}

void DeclarativeManager::slotAdapterRemoved(BluezQt::AdapterPtr adapter)
{
    DeclarativeAdapter *dAdapter = m_adapters.take(adapter->ubi());
    dAdapter->deleteLater();

    Q_EMIT adapterRemoved(dAdapter);
    Q_EMIT adaptersChanged(declarativeAdapters());
}

void DeclarativeManager::slotDeviceAdded(BluezQt::DevicePtr device)
{
    DeclarativeAdapter *dAdapter = declarativeAdapterFromPtr(device->adapter());
    DeclarativeDevice *dDevice = new DeclarativeDevice(device, dAdapter);
    m_devices[device->ubi()] = dDevice;
    dAdapter->m_devices[device->ubi()] = dDevice;

    Q_EMIT deviceAdded(dDevice);
    Q_EMIT devicesChanged(declarativeDevices());
}

void DeclarativeManager::slotDeviceRemoved(BluezQt::DevicePtr device)
{
    DeclarativeDevice *dDevice = m_devices.take(device->ubi());
    dDevice->adapter()->m_devices.take(device->ubi());
    dDevice->deleteLater();

    Q_EMIT deviceRemoved(dDevice);
    Q_EMIT devicesChanged(declarativeDevices());
}

void DeclarativeManager::slotUsableAdapterChanged(BluezQt::AdapterPtr adapter)
{
    Q_EMIT usableAdapterChanged(declarativeAdapterFromPtr(adapter));
}
