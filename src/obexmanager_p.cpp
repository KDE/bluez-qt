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

#include "obexmanager_p.h"
#include "obexmanager.h"
#include "debug.h"
#include "utils.h"

#include "dbusobjectmanager.h"

#include <QDBusServiceWatcher>

namespace BluezQt
{

typedef org::freedesktop::DBus::ObjectManager DBusObjectManager;

ObexManagerPrivate::ObexManagerPrivate(ObexManager *q)
    : QObject(q)
    , q(q)
    , m_obexClient(0)
    , m_obexAgentManager(0)
    , m_dbusObjectManager(0)
    , m_initialized(false)
    , m_obexRunning(false)
    , m_loaded(false)
{
    qDBusRegisterMetaType<DBusManagerStruct>();
    qDBusRegisterMetaType<QVariantMapMap>();

    m_timer = new QTimer(this);
    m_timer->setSingleShot(true);
    m_timer->setInterval(500);
    connect(m_timer, &QTimer::timeout, this, &ObexManagerPrivate::load);
}

ObexManagerPrivate::~ObexManagerPrivate()
{
    clear();
}

void ObexManagerPrivate::init()
{
    // Keep an eye on bluez.obex service
    QDBusServiceWatcher *serviceWatcher = new QDBusServiceWatcher(Strings::orgBluezObex(), DBusConnection::orgBluezObex(),
            QDBusServiceWatcher::WatchForRegistration | QDBusServiceWatcher::WatchForUnregistration, this);

    connect(serviceWatcher, &QDBusServiceWatcher::serviceRegistered, this, &ObexManagerPrivate::serviceRegistered);
    connect(serviceWatcher, &QDBusServiceWatcher::serviceUnregistered, this, &ObexManagerPrivate::serviceUnregistered);

    // Update the current state of bluez.obex service
    if (!DBusConnection::orgBluezObex().isConnected()) {
        Q_EMIT initError(QStringLiteral("DBus session bus is not connected!"));
        return;
    }

    QDBusMessage call = QDBusMessage::createMethodCall(Strings::orgFreedesktopDBus(),
                        QStringLiteral("/"),
                        Strings::orgFreedesktopDBus(),
                        QStringLiteral("NameHasOwner"));

    call << Strings::orgBluezObex();

    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(DBusConnection::orgBluezObex().asyncCall(call));
    connect(watcher, &QDBusPendingCallWatcher::finished, this, &ObexManagerPrivate::nameHasOwnerFinished);
}

void ObexManagerPrivate::nameHasOwnerFinished(QDBusPendingCallWatcher *watcher)
{
    const QDBusPendingReply<bool> &reply = *watcher;
    watcher->deleteLater();

    if (reply.isError()) {
        Q_EMIT initError(reply.error().message());
        return;
    }

    m_obexRunning = reply.value();

    if (m_obexRunning) {
        load();
    } else {
        m_initialized = true;
        Q_EMIT initFinished();
    }
}

void ObexManagerPrivate::load()
{
    if (!m_obexRunning || m_loaded) {
        return;
    }

    m_dbusObjectManager = new DBusObjectManager(Strings::orgBluezObex(), QStringLiteral("/"),
            DBusConnection::orgBluezObex(), this);

    connect(m_dbusObjectManager, &DBusObjectManager::InterfacesRemoved,
            this, &ObexManagerPrivate::interfacesRemoved);

    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(m_dbusObjectManager->GetManagedObjects(), this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, &ObexManagerPrivate::getManagedObjectsFinished);
}

void ObexManagerPrivate::getManagedObjectsFinished(QDBusPendingCallWatcher *watcher)
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

        if (interfaces.contains(Strings::orgBluezObexClient1())) {
            m_obexClient = new ObexClient(Strings::orgBluezObex(), path, DBusConnection::orgBluezObex(), this);
        }
        if (interfaces.contains(Strings::orgBluezObexAgentManager1())) {
            m_obexAgentManager = new ObexAgentManager(Strings::orgBluezObex(), path, DBusConnection::orgBluezObex(), this);
        }
    }

    if (!m_obexClient) {
        Q_EMIT initError(QStringLiteral("Cannot find org.bluez.obex.Client1 object!"));
        return;
    }

    if (!m_obexAgentManager) {
        Q_EMIT initError(QStringLiteral("Cannot find org.bluez.obex.AgentManager1 object!"));
        return;
    }

    m_loaded = true;
    m_initialized = true;

    Q_EMIT q->operationalChanged(true);
    Q_EMIT initFinished();
}

void ObexManagerPrivate::clear()
{
    m_loaded = false;

    if (m_obexClient) {
        m_obexClient->deleteLater();
        m_obexClient = Q_NULLPTR;
    }

    if (m_obexAgentManager) {
        m_obexAgentManager->deleteLater();
        m_obexAgentManager = Q_NULLPTR;
    }

    if (m_dbusObjectManager) {
        m_dbusObjectManager->deleteLater();
        m_dbusObjectManager = Q_NULLPTR;
    }
}

void ObexManagerPrivate::serviceRegistered()
{
    qCDebug(BLUEZQT) << "Obex service registered";
    m_obexRunning = true;

    // Client1 and AgentManager1 objects are not ready by the time org.bluez.obex is registered
    // nor will the ObjectManager emits interfacesAdded for adding them...
    // So we delay the call to load() by 0.5s
    m_timer->start();
}

void ObexManagerPrivate::serviceUnregistered()
{
    qCDebug(BLUEZQT) << "Obex service unregistered";
    m_obexRunning = false;

    clear();
    Q_EMIT q->operationalChanged(false);
}

void ObexManagerPrivate::interfacesRemoved(const QDBusObjectPath &objectPath, const QStringList &interfaces)
{
    if (interfaces.contains(Strings::orgBluezObexSession1())) {
        Q_EMIT q->sessionRemoved(objectPath);
    }
}

} // namespace BluezQt
