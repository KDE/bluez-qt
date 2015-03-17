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

#include "adapter.h"
#include "adapter_p.h"
#include "device.h"
#include "device_p.h"
#include "pendingcall.h"

namespace BluezQt
{

Adapter::Adapter(const QString &path, const QVariantMap &properties)
    : QObject()
    , d(new AdapterPrivate(path, properties))
{
}

Adapter::~Adapter()
{
    delete d;
}

AdapterPtr Adapter::toSharedPtr() const
{
    return d->q.toStrongRef();
}

QString Adapter::ubi() const
{
    return d->m_bluezAdapter->path();
}

QString Adapter::address() const
{
    return d->m_address;
}

QString Adapter::name() const
{
    return d->m_alias;
}

PendingCall *Adapter::setName(const QString &name)
{
    return new PendingCall(d->setDBusProperty(QStringLiteral("Alias"), name),
                           PendingCall::ReturnVoid, this);
}

QString Adapter::systemName() const
{
    return d->m_name;
}

quint32 Adapter::adapterClass() const
{
    return d->m_adapterClass;
}

bool Adapter::isPowered() const
{
    return d->m_powered;
}

PendingCall *Adapter::setPowered(bool powered)
{
    return new PendingCall(d->setDBusProperty(QStringLiteral("Powered"), powered),
                           PendingCall::ReturnVoid, this);
}

bool Adapter::isDiscoverable() const
{
    return d->m_discoverable;
}

PendingCall *Adapter::setDiscoverable(bool discoverable)
{
    return new PendingCall(d->setDBusProperty(QStringLiteral("Discoverable"), discoverable),
                           PendingCall::ReturnVoid, this);
}

quint32 Adapter::discoverableTimeout() const
{
    return d->m_discoverableTimeout;
}

PendingCall *Adapter::setDiscoverableTimeout(quint32 timeout)
{
    return new PendingCall(d->setDBusProperty(QStringLiteral("DiscoverableTimeout"), timeout),
                           PendingCall::ReturnVoid, this);
}

bool Adapter::isPairable() const
{
    return d->m_pairable;
}

PendingCall *Adapter::setPairable(bool pairable)
{
    return new PendingCall(d->setDBusProperty(QStringLiteral("Pairable"), pairable),
                           PendingCall::ReturnVoid, this);
}

quint32 Adapter::pairableTimeout() const
{
    return d->m_pairableTimeout;
}

PendingCall *Adapter::setPairableTimeout(quint32 timeout)
{
    return new PendingCall(d->setDBusProperty(QStringLiteral("PairableTimeout"), timeout),
                           PendingCall::ReturnVoid, this);
}

bool Adapter::isDiscovering()
{
    return d->m_discovering;
}

QStringList Adapter::uuids() const
{
    return d->m_uuids;
}

QString Adapter::modalias() const
{
    return d->m_modalias;
}

QList<DevicePtr> Adapter::devices() const
{
    return d->m_devices;
}

DevicePtr Adapter::deviceForAddress(const QString &address) const
{
    Q_FOREACH (DevicePtr device, d->m_devices) {
        if (device->address() == address) {
            return device;
        }
    }
    return DevicePtr();
}

PendingCall *Adapter::startDiscovery()
{
    return new PendingCall(d->m_bluezAdapter->StartDiscovery(),
                           PendingCall::ReturnVoid, this);
}

PendingCall *Adapter::stopDiscovery()
{
    return new PendingCall(d->m_bluezAdapter->StopDiscovery(),
                           PendingCall::ReturnVoid, this);
}

PendingCall *Adapter::removeDevice(DevicePtr device)
{
    return new PendingCall(d->m_bluezAdapter->RemoveDevice(QDBusObjectPath(device->ubi())),
                           PendingCall::ReturnVoid, this);
}

} // namespace BluezQt
