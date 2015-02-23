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
#include "device.h"

static int devicesCountFunction(QQmlListProperty<BluezQt::Device> *property)
{
    Q_ASSERT(qobject_cast<DeclarativeAdapter*>(property->object));
    DeclarativeAdapter *adapter = static_cast<DeclarativeAdapter*>(property->object);

    return adapter->m_adapter->devices().count();
}

static BluezQt::Device *devicesAtFunction(QQmlListProperty<BluezQt::Device> *property, int index)
{
    Q_ASSERT(qobject_cast<DeclarativeAdapter*>(property->object));
    DeclarativeAdapter *adapter = static_cast<DeclarativeAdapter*>(property->object);

    return adapter->m_adapter->devices().at(index).data();
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

    connect(m_adapter.data(), &BluezQt::Adapter::adapterChanged, [ this ]() {
        Q_EMIT adapterChanged(this);
    });

    connect(m_adapter.data(), &BluezQt::Adapter::deviceFound, [ this ](BluezQt::DevicePtr device) {
        Q_EMIT deviceFound(device.data());
        Q_EMIT devicesChanged(devices());
    });

    connect(m_adapter.data(), &BluezQt::Adapter::deviceRemoved, [ this ](BluezQt::DevicePtr device) {
        Q_EMIT deviceRemoved(device.data());
        Q_EMIT devicesChanged(devices());
    });

    connect(m_adapter.data(), &BluezQt::Adapter::deviceChanged, [ this ](BluezQt::DevicePtr device) {
        Q_EMIT deviceChanged(device.data());
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

BluezQt::PendingCall *DeclarativeAdapter::setName(const QString &name)
{
    return m_adapter->setName(name);
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

BluezQt::PendingCall *DeclarativeAdapter::setPowered(bool powered)
{
    return m_adapter->setPowered(powered);
}

bool DeclarativeAdapter::isDiscoverable() const
{
    return m_adapter->isDiscoverable();
}

BluezQt::PendingCall *DeclarativeAdapter::setDiscoverable(bool discoverable)
{
    return m_adapter->setDiscoverable(discoverable);
}

quint32 DeclarativeAdapter::discoverableTimeout() const
{
    return m_adapter->discoverableTimeout();
}

BluezQt::PendingCall *DeclarativeAdapter::setDiscoverableTimeout(quint32 timeout)
{
    return m_adapter->setDiscoverableTimeout(timeout);
}

bool DeclarativeAdapter::isPairable() const
{
    return m_adapter->isPairable();
}

BluezQt::PendingCall *DeclarativeAdapter::setPairable(bool pairable)
{
    return m_adapter->setPairable(pairable);
}

quint32 DeclarativeAdapter::pairableTimeout() const
{
    return m_adapter->pairableTimeout();
}

BluezQt::PendingCall *DeclarativeAdapter::setPairableTimeout(quint32 timeout)
{
    return m_adapter->setPairableTimeout(timeout);
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

QQmlListProperty<BluezQt::Device> DeclarativeAdapter::devices()
{
    return QQmlListProperty<BluezQt::Device>(this, 0, devicesCountFunction, devicesAtFunction);
}

BluezQt::Device *DeclarativeAdapter::deviceForAddress(const QString &address) const
{
    return m_adapter->deviceForAddress(address).data();
}

BluezQt::PendingCall *DeclarativeAdapter::startDiscovery()
{
    return m_adapter->startDiscovery();
}

BluezQt::PendingCall *DeclarativeAdapter::stopDiscovery()
{
    return m_adapter->stopDiscovery();
}

BluezQt::PendingCall *DeclarativeAdapter::removeDevice(BluezQt::Device *device)
{
    return m_adapter->removeDevice(m_adapter->deviceForAddress(device->address()));
}
