/*
 * Copyright (C) 2019 Manuel Weichselbaumer <mincequi@web.de>
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

#include "gattmanagerinterface.h"
#include "objectmanager.h"

#include <QDebug>
#include <QDBusMessage>
#include <QDBusConnection>
#include <QDBusPendingCall>
#include <QDBusPendingReply>

GattManagerInterface::GattManagerInterface(const QDBusObjectPath &path, QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    setName(QStringLiteral("org.bluez.GattManager1"));
    setPath(path);
}

void GattManagerInterface::runAction(const QString &actionName, const QVariantMap &properties)
{
    if (actionName == QLatin1String("get-objects")) {
        runGetObjectsAction();
    } else if (actionName == QLatin1String("read-charc")) {
        runReadCharcAction(properties);
    } else if (actionName == QLatin1String("write-charc")) {
        runWriteCharcAction(properties);
    }
}

void GattManagerInterface::RegisterApplication(const QDBusObjectPath &path, const QVariantMap &/*options*/, const QDBusMessage &msg)
{
    m_application = path;
    m_service = msg.service();
}

void GattManagerInterface::UnregisterApplication(const QDBusObjectPath &path, const QDBusMessage &msg)
{
    if (m_application == path && m_service == msg.service()) {
        m_application = QDBusObjectPath();
        m_service.clear();
    }
}

void GattManagerInterface::runGetObjectsAction()
{
    QDBusMessage call = QDBusMessage::createMethodCall(m_service,
                                                       m_application.path(),
                                                       QStringLiteral("org.freedesktop.DBus.ObjectManager"),
                                                       QStringLiteral("GetManagedObjects"));

    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(QDBusConnection::sessionBus().asyncCall(call));
    connect(watcher, &QDBusPendingCallWatcher::finished, [this](QDBusPendingCallWatcher *watcher) {
        const QDBusPendingReply<DBusManagerStruct> &reply = *watcher;
        watcher->deleteLater();
        if (reply.isError()) {
            return;
        }

        DBusManagerStruct objects = reply.value();
        for (const auto& object : objects.keys()) {
            if (object.path().contains(QStringLiteral("char"))) {
                m_characteristic = object;
                break;
            }
        }
    });
}

void GattManagerInterface::runReadCharcAction(const QVariantMap &properties)
{
    QDBusMessage call = QDBusMessage::createMethodCall(m_service,
                                                       m_characteristic.path(),
                                                       QStringLiteral("org.bluez.GattCharacteristic1"),
                                                       QStringLiteral("ReadValue"));
    call << properties.value(QStringLiteral("Options"));
    QDBusConnection::sessionBus().asyncCall(call);
}

void GattManagerInterface::runWriteCharcAction(const QVariantMap &properties)
{
    QDBusMessage call = QDBusMessage::createMethodCall(m_service,
                                                       m_characteristic.path(),
                                                       QStringLiteral("org.bluez.GattCharacteristic1"),
                                                       QStringLiteral("WriteValue"));
    call << properties.value(QStringLiteral("Value"));
    call << properties.value(QStringLiteral("Options"));
    QDBusConnection::sessionBus().asyncCall(call);
}
