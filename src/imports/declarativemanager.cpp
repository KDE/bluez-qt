/*
 * BluezQt - Asynchronous Bluez wrapper library
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

static int adaptersCountFunction(QQmlListProperty<BluezQt::Adapter> *property)
{
    Q_ASSERT(qobject_cast<DeclarativeManager*>(property->object));
    DeclarativeManager *manager = static_cast<DeclarativeManager*>(property->object);

    return manager->adapters().count();
}

static BluezQt::Adapter *adaptersAtFunction(QQmlListProperty<BluezQt::Adapter> *property, int index)
{
    Q_ASSERT(qobject_cast<DeclarativeManager*>(property->object));
    DeclarativeManager *manager = static_cast<DeclarativeManager*>(property->object);

    return manager->adapters().at(index).data();
}

static int devicesCountFunction(QQmlListProperty<BluezQt::Device> *property)
{
    Q_ASSERT(qobject_cast<DeclarativeManager*>(property->object));
    DeclarativeManager *manager = static_cast<DeclarativeManager*>(property->object);

    return manager->devices().count();
}

static BluezQt::Device *devicesAtFunction(QQmlListProperty<BluezQt::Device> *property, int index)
{
    Q_ASSERT(qobject_cast<DeclarativeManager*>(property->object));
    DeclarativeManager *manager = static_cast<DeclarativeManager*>(property->object);

    return manager->devices().at(index).data();
}

DeclarativeManager::DeclarativeManager(QObject *parent)
    : BluezQt::Manager(parent)
{
    if (s_instance) {
        qWarning("DeclarativeManager: Only one instance is allowed!");
    }
    s_instance = this;

    BluezQt::InitManagerJob *job = init();
    job->start();
    connect(job, &BluezQt::InitManagerJob::result, this, &DeclarativeManager::initJobResult);

    connect(this, SIGNAL(adapterAdded(BluezQt::AdapterPtr)), this, SLOT(slotAdapterAdded(BluezQt::AdapterPtr)));
    connect(this, SIGNAL(adapterRemoved(BluezQt::AdapterPtr)), this, SLOT(slotAdapterRemoved(BluezQt::AdapterPtr)));
    connect(this, SIGNAL(usableAdapterChanged(BluezQt::AdapterPtr)), this, SLOT(slotUsableAdapterChanged(BluezQt::AdapterPtr)));
}

BluezQt::Adapter *DeclarativeManager::usableAdapter() const
{
    return BluezQt::Manager::usableAdapter().data();
}

QQmlListProperty<BluezQt::Adapter> DeclarativeManager::declarativeAdapters()
{
    return QQmlListProperty<BluezQt::Adapter>(this, 0, adaptersCountFunction, adaptersAtFunction);
}

QQmlListProperty<BluezQt::Device> DeclarativeManager::declarativeDevices()
{
    return QQmlListProperty<BluezQt::Device>(this, 0, devicesCountFunction, devicesAtFunction);
}

DeclarativeManager *DeclarativeManager::self()
{
    return s_instance;
}

BluezQt::Adapter *DeclarativeManager::adapterForAddress(const QString &address) const
{
    return BluezQt::Manager::adapterForAddress(address).data();
}

BluezQt::Adapter *DeclarativeManager::adapterForUbi(const QString &ubi) const
{
    return BluezQt::Manager::adapterForUbi(ubi).data();
}

BluezQt::Device *DeclarativeManager::deviceForAddress(const QString &address) const
{
    return BluezQt::Manager::deviceForAddress(address).data();
}

BluezQt::Device *DeclarativeManager::deviceForUbi(const QString &ubi) const
{
    return BluezQt::Manager::deviceForUbi(ubi).data();
}

void DeclarativeManager::initJobResult(BluezQt::InitManagerJob *job)
{
    if (job->error()) {
        Q_EMIT initializeError(job->errorText());
        return;
    }

    Q_EMIT initialized();
}

void DeclarativeManager::slotAdapterAdded(BluezQt::AdapterPtr adapter)
{
    Q_EMIT adaptersChanged(declarativeAdapters());

    connect(adapter.data(), &BluezQt::Adapter::deviceFound, this, &DeclarativeManager::slotDeviceAdded);
    connect(adapter.data(), &BluezQt::Adapter::deviceRemoved, this, &DeclarativeManager::slotDeviceRemoved);
}

void DeclarativeManager::slotAdapterRemoved(BluezQt::AdapterPtr adapter)
{
    Q_EMIT adaptersChanged(declarativeAdapters());

    disconnect(adapter.data(), &BluezQt::Adapter::deviceFound, this, &DeclarativeManager::slotDeviceAdded);
    disconnect(adapter.data(), &BluezQt::Adapter::deviceRemoved, this, &DeclarativeManager::slotDeviceRemoved);
}

void DeclarativeManager::slotDeviceAdded(BluezQt::DevicePtr device)
{
    Q_UNUSED(device)
    Q_EMIT devicesChanged(declarativeDevices());
}

void DeclarativeManager::slotDeviceRemoved(BluezQt::DevicePtr device)
{
    Q_UNUSED(device)
    Q_EMIT devicesChanged(declarativeDevices());
}

void DeclarativeManager::slotUsableAdapterChanged(BluezQt::AdapterPtr adapter)
{
    Q_EMIT usableAdapterChanged(adapter.data());
}
