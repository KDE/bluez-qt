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

#ifndef QBLUEZ_MANAGER_P_H
#define QBLUEZ_MANAGER_P_H

#include <QObject>
#include <QHash>

#include "dbusobjectmanager.h"
#include "bluezagentmanager1.h"

namespace QBluez
{

typedef org::freedesktop::DBus::ObjectManager DBusObjectManager;
typedef org::bluez::AgentManager1 BluezAgentManager;

class Manager;
class Adapter;
class Device;
class AdapterPrivate;

class ManagerPrivate : public QObject
{
    Q_OBJECT

public:
    explicit ManagerPrivate(Manager *parent);

    void init();
    void nameHasOwnerFinished(QDBusPendingCallWatcher *watcher);
    void load();
    void getManagedObjectsFinished(QDBusPendingCallWatcher *watcher);
    void clear();

    Adapter *findUsableAdapter() const;

    void serviceRegistered();
    void serviceUnregistered();
    void interfacesAdded(const QDBusObjectPath &objectPath, const QVariantMapMap &interfaces);
    void interfacesRemoved(const QDBusObjectPath &objectPath, const QStringList &interfaces);
    void adapterRemoved(Adapter *adapter);
    void adapterPoweredChanged(bool powered);

    void addAdapter(const QString &adapterPath, const QVariantMap &properties);
    void addDevice(const QString &devicePath, const QVariantMap &properties);
    void removeAdapter(const QString &adapterPath);
    void removeDevice(const QString &devicePath);
    void setUsableAdapter(Adapter *adapter);

    Manager *q;
    DBusObjectManager *m_dbusObjectManager;
    BluezAgentManager *m_bluezAgentManager;

    QHash<QString, Adapter*> m_adapters;
    QHash<QString, Device*> m_devices;
    Adapter *m_usableAdapter;

    bool m_initialized;
    bool m_bluezRunning;
    bool m_loaded;
    bool m_adaptersLoaded;

Q_SIGNALS:
    void initError(const QString &errorText);
    void initFinished();
};

} // namespace QBluez

#endif // QBLUEZ_MANAGER_P_H
