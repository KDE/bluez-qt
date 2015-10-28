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

#include "manager.h"
#include "manager_p.h"
#include "adapter.h"
#include "agent.h"
#include "agentadaptor.h"
#include "profile.h"
#include "profile_p.h"
#include "profileadaptor.h"
#include "pendingcall.h"
#include "initmanagerjob.h"
#include "utils.h"
#include "debug.h"

namespace BluezQt
{

Manager::Manager(QObject *parent)
    : QObject(parent)
    , d(new ManagerPrivate(this))
{
    Instance::setManager(this);
}

Manager::~Manager()
{
    delete d;
}

InitManagerJob *Manager::init()
{
    return new InitManagerJob(this);
}

bool Manager::isInitialized() const
{
    return d->m_initialized;
}

bool Manager::isOperational() const
{
    return d->m_initialized && d->m_bluezRunning && d->m_loaded;
}

bool Manager::isBluetoothOperational() const
{
    return !d->m_bluetoothBlocked && d->m_bluezRunning && d->m_loaded && d->m_usableAdapter;
}

bool Manager::isBluetoothBlocked() const
{
    return d->m_bluetoothBlocked;
}

bool Manager::setBluetoothBlocked(bool blocked)
{
    if (blocked) {
        return d->m_rfkill->block();
    } else {
        return d->m_rfkill->unblock();
    }
}

AdapterPtr Manager::usableAdapter() const
{
    return d->m_usableAdapter;
}

QList<AdapterPtr> Manager::adapters() const
{
    return d->m_adapters.values();
}

QList<DevicePtr> Manager::devices() const
{
    return d->m_devices.values();
}

PendingCall *Manager::startService()
{
    QDBusMessage msg = QDBusMessage::createMethodCall(Strings::orgFreedesktopDBus(),
                                                      QStringLiteral("/org/freedesktop/DBus"),
                                                      Strings::orgFreedesktopDBus(),
                                                      QStringLiteral("StartServiceByName"));
    msg << Strings::orgBluez();
    msg << quint32(0);

    return new PendingCall(DBusConnection::orgBluez().asyncCall(msg), PendingCall::ReturnUint32);
}

AdapterPtr Manager::adapterForAddress(const QString &address) const
{
    Q_FOREACH (AdapterPtr adapter, d->m_adapters) {
        if (adapter->address() == address) {
            return adapter;
        }
    }
    return AdapterPtr();
}

AdapterPtr Manager::adapterForUbi(const QString &ubi) const
{
    return d->m_adapters.value(ubi);
}

DevicePtr Manager::deviceForAddress(const QString &address) const
{
    DevicePtr device;

    Q_FOREACH (AdapterPtr adapter, d->m_adapters) {
        DevicePtr d = adapter->deviceForAddress(address);
        if (!d) {
            continue;
        }

        // Prefer powered adapter
        if (!device) {
            device = d;
        } else if (adapter->isPowered()) {
            device = d;
        }
    }

    return device;
}

DevicePtr Manager::deviceForUbi(const QString &ubi) const
{
    return d->m_devices.value(ubi);
}

PendingCall *Manager::registerAgent(Agent *agent)
{
    Q_ASSERT(agent);

    if (!d->m_bluezAgentManager) {
        return new PendingCall(PendingCall::InternalError, QStringLiteral("Manager not operational!"));
    }

    QString capability;

    switch (agent->capability()) {
    case Agent::DisplayOnly:
        capability = QStringLiteral("DisplayOnly");
        break;
    case Agent::DisplayYesNo:
        capability = QStringLiteral("DisplayYesNo");
        break;
    case Agent::KeyboardOnly:
        capability = QStringLiteral("KeyboardOnly");
        break;
    case Agent::NoInputNoOutput:
        capability = QStringLiteral("NoInputNoOutput");
        break;
    default:
        capability = QStringLiteral("DisplayYesNo");
        break;
    }

    new AgentAdaptor(agent, this);

    if (!DBusConnection::orgBluez().registerObject(agent->objectPath().path(), agent)) {
        qCDebug(BLUEZQT) << "Cannot register object" << agent->objectPath().path();
    }

    return new PendingCall(d->m_bluezAgentManager->RegisterAgent(agent->objectPath(), capability),
                           PendingCall::ReturnVoid, this);
}

PendingCall *Manager::unregisterAgent(Agent *agent)
{
    Q_ASSERT(agent);

    if (!d->m_bluezAgentManager) {
        return new PendingCall(PendingCall::InternalError, QStringLiteral("Manager not operational!"));
    }

    DBusConnection::orgBluez().unregisterObject(agent->objectPath().path());

    return new PendingCall(d->m_bluezAgentManager->UnregisterAgent(agent->objectPath()),
                           PendingCall::ReturnVoid, this);
}

PendingCall *Manager::requestDefaultAgent(Agent *agent)
{
    Q_ASSERT(agent);

    if (!d->m_bluezAgentManager) {
        return new PendingCall(PendingCall::InternalError, QStringLiteral("Manager not operational!"));
    }

    return new PendingCall(d->m_bluezAgentManager->RequestDefaultAgent(agent->objectPath()),
                           PendingCall::ReturnVoid, this);
}

PendingCall *Manager::registerProfile(Profile *profile)
{
    Q_ASSERT(profile);

    if (!d->m_bluezProfileManager) {
        return new PendingCall(PendingCall::InternalError, QStringLiteral("Manager not operational!"));
    }

    new ProfileAdaptor(profile, this);

    if (!DBusConnection::orgBluez().registerObject(profile->objectPath().path(), profile)) {
        qCDebug(BLUEZQT) << "Cannot register object" << profile->objectPath().path();
    }

    return new PendingCall(d->m_bluezProfileManager->RegisterProfile(profile->objectPath(), profile->uuid(), profile->d->options),
                           PendingCall::ReturnVoid, this);
}

PendingCall *Manager::unregisterProfile(Profile *profile)
{
    Q_ASSERT(profile);

    if (!d->m_bluezProfileManager) {
        return new PendingCall(PendingCall::InternalError, QStringLiteral("Manager not operational!"));
    }

    DBusConnection::orgBluez().unregisterObject(profile->objectPath().path());

    return new PendingCall(d->m_bluezProfileManager->UnregisterProfile(profile->objectPath()),
                           PendingCall::ReturnVoid, this);
}

} // namespace BluezQt
