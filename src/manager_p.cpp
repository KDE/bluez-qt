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

#include "manager_p.h"
#include "manager.h"
#include "device.h"
#include "device_p.h"
#include "adapter.h"
#include "adapter_p.h"
#include "debug_p.h"
#include "utils_p.h"

#include <QDBusReply>
#include <QDBusConnection>
#include <QDBusServiceWatcher>

namespace BluezQt
{

ManagerPrivate::ManagerPrivate(Manager *parent)
    : QObject(parent)
    , q(parent)
    , m_dbusObjectManager(0)
    , m_bluezAgentManager(0)
    , m_bluezProfileManager(0)
    , m_usableAdapter(0)
    , m_initialized(false)
    , m_bluezRunning(false)
    , m_loaded(false)
    , m_adaptersLoaded(false)
{
    qDBusRegisterMetaType<DBusManagerStruct>();
    qDBusRegisterMetaType<QVariantMapMap>();

    connect(q, &Manager::adapterRemoved, this, &ManagerPrivate::adapterRemoved);
}

void ManagerPrivate::init()
{
    // Keep an eye on bluez service
    QDBusServiceWatcher *serviceWatcher = new QDBusServiceWatcher(Strings::orgBluez(), DBusConnection::orgBluez(),
            QDBusServiceWatcher::WatchForRegistration | QDBusServiceWatcher::WatchForUnregistration, this);

    connect(serviceWatcher, &QDBusServiceWatcher::serviceRegistered, this, &ManagerPrivate::serviceRegistered);
    connect(serviceWatcher, &QDBusServiceWatcher::serviceUnregistered, this, &ManagerPrivate::serviceUnregistered);

    // Update the current state of bluez service
    if (!DBusConnection::orgBluez().isConnected()) {
        Q_EMIT initError(QStringLiteral("DBus system bus is not connected!"));
        return;
    }

    QDBusMessage call = QDBusMessage::createMethodCall(Strings::orgFreedesktopDBus(),
                        QStringLiteral("/"),
                        Strings::orgFreedesktopDBus(),
                        QStringLiteral("NameHasOwner"));

    call << Strings::orgBluez();

    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(DBusConnection::orgBluez().asyncCall(call));
    connect(watcher, &QDBusPendingCallWatcher::finished, this, &ManagerPrivate::nameHasOwnerFinished);
}

void ManagerPrivate::nameHasOwnerFinished(QDBusPendingCallWatcher *watcher)
{
    const QDBusPendingReply<bool> &reply = *watcher;
    watcher->deleteLater();

    if (reply.isError()) {
        Q_EMIT initError(reply.error().message());
        return;
    }

    m_bluezRunning = reply.value();

    if (m_bluezRunning) {
        load();
    } else {
        m_initialized = true;
        Q_EMIT initFinished();
    }
}

void ManagerPrivate::load()
{
    if (!m_bluezRunning || m_loaded) {
        return;
    }

    m_dbusObjectManager = new DBusObjectManager(Strings::orgBluez(), QStringLiteral("/"),
            DBusConnection::orgBluez(), this);

    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(m_dbusObjectManager->GetManagedObjects(), this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, &ManagerPrivate::getManagedObjectsFinished);
}

void ManagerPrivate::getManagedObjectsFinished(QDBusPendingCallWatcher *watcher)
{
    const QDBusPendingReply<DBusManagerStruct> &reply = *watcher;
    watcher->deleteLater();

    if (reply.isError()) {
        Q_EMIT initError(reply.error().message());
        return;
    }

    DBusManagerStruct::const_iterator it;
    const DBusManagerStruct &managedObjects = reply.value();

    for (it = managedObjects.constBegin(); it != managedObjects.constEnd(); ++it) {
        const QString &path = it.key().path();
        const QVariantMapMap &interfaces = it.value();

        if (interfaces.contains(Strings::orgBluezAdapter1())) {
            addAdapter(path, interfaces.value(Strings::orgBluezAdapter1()));
        } else if (interfaces.contains(Strings::orgBluezDevice1())) {
            addDevice(path, interfaces.value(Strings::orgBluezDevice1()));
        } else if (interfaces.contains(Strings::orgBluezAgentManager1()) && interfaces.contains(Strings::orgBluezProfileManager1())) {
            m_bluezAgentManager = new BluezAgentManager(Strings::orgBluez(), path, DBusConnection::orgBluez(), this);
            m_bluezProfileManager = new BluezProfileManager(Strings::orgBluez(), path, DBusConnection::orgBluez(), this);
        }
    }

    if (!m_bluezAgentManager) {
        Q_EMIT initError(QStringLiteral("Cannot find org.bluez.AgentManager1 object!"));
        return;
    }

    connect(m_dbusObjectManager, &DBusObjectManager::InterfacesAdded,
            this, &ManagerPrivate::interfacesAdded);
    connect(m_dbusObjectManager, &DBusObjectManager::InterfacesRemoved,
            this, &ManagerPrivate::interfacesRemoved);

    m_loaded = true;
    m_initialized = true;

    Q_EMIT q->operationalChanged(true);

    if (q->isBluetoothOperational()) {
        Q_EMIT q->bluetoothOperationalChanged(true);
    }

    Q_EMIT initFinished();
}

void ManagerPrivate::clear()
{
    m_loaded = false;

    // Delete all devices first
    Q_FOREACH (DevicePtr device, m_devices.values()) {
        device->adapter()->d->removeDevice(device);
    }
    m_devices.clear();

    // Delete all adapters
    while (!m_adapters.isEmpty()) {
        AdapterPtr adapter = m_adapters.begin().value();
        m_adapters.remove(m_adapters.begin().key());
        Q_EMIT adapter->adapterRemoved(adapter);

        if (m_adapters.isEmpty()) {
            Q_EMIT q->allAdaptersRemoved();
        }
    }

    // Delete all other objects
    m_usableAdapter.clear();

    if (m_dbusObjectManager) {
        m_dbusObjectManager->deleteLater();
        m_dbusObjectManager = Q_NULLPTR;
    }

    if (m_bluezAgentManager) {
        m_bluezAgentManager->deleteLater();
        m_bluezAgentManager = Q_NULLPTR;
    }
}

AdapterPtr ManagerPrivate::findUsableAdapter() const
{
    Q_FOREACH (AdapterPtr adapter, m_adapters) {
        if (adapter->isPowered()) {
            return adapter;
        }
    }
    return AdapterPtr();
}

void ManagerPrivate::serviceRegistered()
{
    qCDebug(BLUEZQT) << "Bluez service registered";
    m_bluezRunning = true;

    load();
}

void ManagerPrivate::serviceUnregistered()
{
    qCDebug(BLUEZQT) << "Bluez service unregistered";

    bool wasBtOperational = q->isBluetoothOperational();
    m_bluezRunning = false;

    if (wasBtOperational) {
        Q_EMIT q->bluetoothOperationalChanged(false);
    }

    clear();
    Q_EMIT q->operationalChanged(false);
}

void ManagerPrivate::interfacesAdded(const QDBusObjectPath &objectPath, const QVariantMapMap &interfaces)
{
    const QString &path = objectPath.path();
    QVariantMapMap::const_iterator it;

    for (it = interfaces.constBegin(); it != interfaces.constEnd(); ++it) {
        if (it.key() == Strings::orgBluezAdapter1()) {
            addAdapter(path, it.value());
        } else if (it.key() == Strings::orgBluezDevice1()) {
            addDevice(path, it.value());
        }
    }
}

void ManagerPrivate::interfacesRemoved(const QDBusObjectPath &objectPath, const QStringList &interfaces)
{
    const QString &path = objectPath.path();

    Q_FOREACH (const QString &interface, interfaces) {
        if (interface == Strings::orgBluezAdapter1()) {
            removeAdapter(path);
        } else if (interface == Strings::orgBluezDevice1()) {
            removeDevice(path);
        }
    }
}

void ManagerPrivate::adapterRemoved(AdapterPtr adapter)
{
    disconnect(adapter.data(), &Adapter::poweredChanged, this, &ManagerPrivate::adapterPoweredChanged);

    // Current usable adapter was removed
    if (adapter == m_usableAdapter) {
        setUsableAdapter(findUsableAdapter());
    }
}

void ManagerPrivate::adapterPoweredChanged(bool powered)
{
    Q_ASSERT(qobject_cast<Adapter*>(sender()));
    AdapterPtr adapter = static_cast<Adapter*>(sender())->d->q.toStrongRef();

    // Current usable adapter was powered off
    if (m_usableAdapter == adapter && !powered) {
        setUsableAdapter(findUsableAdapter());
    }

    // Adapter was powered on, set it as usable
    if (!m_usableAdapter && powered) {
        setUsableAdapter(adapter);
    }
}

void ManagerPrivate::addAdapter(const QString &adapterPath, const QVariantMap &properties)
{
    AdapterPtr adapter = AdapterPtr(new Adapter(adapterPath, properties));
    adapter->d->q = adapter.toWeakRef();
    m_adapters.insert(adapterPath, adapter);

    Q_EMIT q->adapterAdded(adapter);

    // Powered adapter was added, set it as usable
    if (!m_usableAdapter && adapter->isPowered()) {
        setUsableAdapter(adapter);
    }

    connect(adapter.data(), &Adapter::deviceAdded, q, &Manager::deviceAdded);
    connect(adapter.data(), &Adapter::adapterRemoved, q, &Manager::adapterRemoved);
    connect(adapter.data(), &Adapter::adapterChanged, q, &Manager::adapterChanged);
    connect(adapter.data(), &Adapter::poweredChanged, this, &ManagerPrivate::adapterPoweredChanged);
}

void ManagerPrivate::addDevice(const QString &devicePath, const QVariantMap &properties)
{
    AdapterPtr adapter = m_adapters.value(properties.value(QStringLiteral("Adapter")).value<QDBusObjectPath>().path());
    Q_ASSERT(adapter);
    DevicePtr device = DevicePtr(new Device(devicePath, properties, adapter));
    m_devices.insert(devicePath, device);
    device->d->q = device.toWeakRef();
    adapter->d->addDevice(device);

    connect(device.data(), &Device::deviceRemoved, q, &Manager::deviceRemoved);
    connect(device.data(), &Device::deviceChanged, q, &Manager::deviceChanged);
}

void ManagerPrivate::removeAdapter(const QString &adapterPath)
{
    AdapterPtr adapter = m_adapters.take(adapterPath);
    if (!adapter) {
        return;
    }

    Q_EMIT adapter->adapterRemoved(adapter);

    if (m_adapters.isEmpty()) {
        Q_EMIT q->allAdaptersRemoved();
    }
}

void ManagerPrivate::removeDevice(const QString &devicePath)
{
    DevicePtr device = m_devices.take(devicePath);
    if (!device) {
        return;
    }

    device->adapter()->d->removeDevice(device);
}

void ManagerPrivate::setUsableAdapter(AdapterPtr adapter)
{
    if (m_usableAdapter == adapter) {
        return;
    }

    bool wasBtOperational = q->isBluetoothOperational();

    m_usableAdapter = adapter;
    Q_EMIT q->usableAdapterChanged(m_usableAdapter);

    if (wasBtOperational != q->isBluetoothOperational()) {
        Q_EMIT q->bluetoothOperationalChanged(q->isBluetoothOperational());
    }
}

} // namespace BluezQt
