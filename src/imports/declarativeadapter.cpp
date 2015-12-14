/*
 * BluezQt - Asynchronous Bluez wrapper library
 *
 * Copyright (C) 2015 David Rosca <nowrep@gmail.com>
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

#include "declarativeadapter.h"
#include "declarativedevice.h"

static int devicesCountFunction(QQmlListProperty<DeclarativeDevice> *property)
{
    Q_ASSERT(qobject_cast<DeclarativeAdapter*>(property->object));
    DeclarativeAdapter *adapter = static_cast<DeclarativeAdapter*>(property->object);

    return adapter->m_devices.count();
}

static DeclarativeDevice *devicesAtFunction(QQmlListProperty<DeclarativeDevice> *property, int index)
{
    Q_ASSERT(qobject_cast<DeclarativeAdapter*>(property->object));
    DeclarativeAdapter *adapter = static_cast<DeclarativeAdapter*>(property->object);

    return adapter->m_devices.values().at(index);
}

DeclarativeAdapter::DeclarativeAdapter(BluezQt::AdapterPtr adapter, QObject *parent)
    : QObject(parent)
    , m_adapter(adapter)
{
    connect(m_adapter.data(), &BluezQt::Adapter::nameChanged, this, &DeclarativeAdapter::nameChanged);
    connect(m_adapter.data(), &BluezQt::Adapter::systemNameChanged, this, &DeclarativeAdapter::systemNameChanged);
    connect(m_adapter.data(), &BluezQt::Adapter::adapterClassChanged, this, &DeclarativeAdapter::adapterClassChanged);
    connect(m_adapter.data(), &BluezQt::Adapter::poweredChanged, this, &DeclarativeAdapter::poweredChanged);
    connect(m_adapter.data(), &BluezQt::Adapter::discoverableChanged, this, &DeclarativeAdapter::discoverableChanged);
    connect(m_adapter.data(), &BluezQt::Adapter::discoverableTimeoutChanged, this, &DeclarativeAdapter::discoverableTimeoutChanged);
    connect(m_adapter.data(), &BluezQt::Adapter::pairableChanged, this, &DeclarativeAdapter::pairableChanged);
    connect(m_adapter.data(), &BluezQt::Adapter::pairableTimeoutChanged, this, &DeclarativeAdapter::pairableTimeoutChanged);
    connect(m_adapter.data(), &BluezQt::Adapter::discoveringChanged, this, &DeclarativeAdapter::discoveringChanged);
    connect(m_adapter.data(), &BluezQt::Adapter::uuidsChanged, this, &DeclarativeAdapter::uuidsChanged);
    connect(m_adapter.data(), &BluezQt::Adapter::modaliasChanged, this, &DeclarativeAdapter::modaliasChanged);

    connect(m_adapter.data(), &BluezQt::Adapter::deviceAdded, this, &DeclarativeAdapter::slotDeviceAdded);
    connect(m_adapter.data(), &BluezQt::Adapter::deviceRemoved, this, &DeclarativeAdapter::slotDeviceRemoved);

    connect(m_adapter.data(), &BluezQt::Adapter::adapterRemoved, this, [this]() {
        Q_EMIT adapterRemoved(this);
    });

    connect(m_adapter.data(), &BluezQt::Adapter::adapterChanged, this, [this]() {
        Q_EMIT adapterChanged(this);
    });

    connect(m_adapter.data(), &BluezQt::Adapter::deviceChanged, this, [this](const BluezQt::DevicePtr &device) {
        Q_EMIT deviceChanged(declarativeDeviceFromPtr(device));
    });
}

QString DeclarativeAdapter::ubi() const
{
    return m_adapter->ubi();
}

QString DeclarativeAdapter::address() const
{
    return m_adapter->address();
}

QString DeclarativeAdapter::name() const
{
    return m_adapter->name();
}

void DeclarativeAdapter::setName(const QString &name)
{
    m_adapter->setName(name);
}

QString DeclarativeAdapter::systemName() const
{
    return m_adapter->systemName();
}

quint32 DeclarativeAdapter::adapterClass() const
{
    return m_adapter->adapterClass();
}

bool DeclarativeAdapter::isPowered() const
{
    return m_adapter->isPowered();
}

void DeclarativeAdapter::setPowered(bool powered)
{
    m_adapter->setPowered(powered);
}

bool DeclarativeAdapter::isDiscoverable() const
{
    return m_adapter->isDiscoverable();
}

void DeclarativeAdapter::setDiscoverable(bool discoverable)
{
    m_adapter->setDiscoverable(discoverable);
}

quint32 DeclarativeAdapter::discoverableTimeout() const
{
    return m_adapter->discoverableTimeout();
}

void DeclarativeAdapter::setDiscoverableTimeout(quint32 timeout)
{
    m_adapter->setDiscoverableTimeout(timeout);
}

bool DeclarativeAdapter::isPairable() const
{
    return m_adapter->isPairable();
}

void DeclarativeAdapter::setPairable(bool pairable)
{
    m_adapter->setPairable(pairable);
}

quint32 DeclarativeAdapter::pairableTimeout() const
{
    return m_adapter->pairableTimeout();
}

void DeclarativeAdapter::setPairableTimeout(quint32 timeout)
{
    m_adapter->setPairableTimeout(timeout);
}

bool DeclarativeAdapter::isDiscovering()
{
    return m_adapter->isDiscovering();
}

QStringList DeclarativeAdapter::uuids() const
{
    return m_adapter->uuids();
}

QString DeclarativeAdapter::modalias() const
{
    return m_adapter->modalias();
}

QQmlListProperty<DeclarativeDevice> DeclarativeAdapter::devices()
{
    return QQmlListProperty<DeclarativeDevice>(this, 0, devicesCountFunction, devicesAtFunction);
}

DeclarativeDevice *DeclarativeAdapter::deviceForAddress(const QString &address) const
{
    return declarativeDeviceFromPtr(m_adapter->deviceForAddress(address));
}

BluezQt::PendingCall *DeclarativeAdapter::startDiscovery()
{
    return m_adapter->startDiscovery();
}

BluezQt::PendingCall *DeclarativeAdapter::stopDiscovery()
{
    return m_adapter->stopDiscovery();
}

BluezQt::PendingCall *DeclarativeAdapter::removeDevice(DeclarativeDevice *device)
{
    return m_adapter->removeDevice(m_adapter->deviceForAddress(device->address()));
}

void DeclarativeAdapter::slotDeviceAdded(BluezQt::DevicePtr device)
{
    Q_EMIT deviceFound(declarativeDeviceFromPtr(device));
    Q_EMIT devicesChanged(devices());
}

void DeclarativeAdapter::slotDeviceRemoved(BluezQt::DevicePtr device)
{
    Q_EMIT deviceRemoved(declarativeDeviceFromPtr(device));
    Q_EMIT devicesChanged(devices());
}

DeclarativeDevice *DeclarativeAdapter::declarativeDeviceFromPtr(BluezQt::DevicePtr ptr) const
{
    if (!ptr) {
        return Q_NULLPTR;
    }
    return m_devices.value(ptr->ubi());
}
