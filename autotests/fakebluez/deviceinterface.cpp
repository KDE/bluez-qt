/*
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

#include "deviceinterface.h"

#include <QTimer>
#include <QDBusConnection>

// DeviceObject
DeviceObject::DeviceObject(const QDBusObjectPath &path, QObject *parent)
    : QObject(parent)
{
    QDBusConnection::sessionBus().registerObject(path.path(), this);
}

// DeviceInterface
DeviceInterface::DeviceInterface(const QDBusObjectPath &path, const QVariantMap &properties, QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    setPath(path);
    setProperties(properties);
    setName(QStringLiteral("org.bluez.Device1"));

    // Alias needs special handling
    setAlias(properties.value(QStringLiteral("Alias")).toString());
}

QString DeviceInterface::address() const
{
    return Object::property(QStringLiteral("Address")).toString();
}

QString DeviceInterface::name() const
{
    return Object::property(QStringLiteral("Name")).toString();
}

QString DeviceInterface::alias() const
{
    return Object::property(QStringLiteral("Alias")).toString();
}

void DeviceInterface::setAlias(const QString &alias)
{
    Object::changeProperty(QStringLiteral("Alias"), alias.isEmpty() ? name() : alias);
}

QString DeviceInterface::icon() const
{
    return Object::property(QStringLiteral("Icon")).toString();
}

quint32 DeviceInterface::deviceClass() const
{
    return Object::property(QStringLiteral("Class")).toUInt();
}

quint16 DeviceInterface::appearance() const
{
    return Object::property(QStringLiteral("Appearance")).toUInt();
}

bool DeviceInterface::paired() const
{
    return Object::property(QStringLiteral("Paired")).toBool();
}

bool DeviceInterface::connected() const
{
    return Object::property(QStringLiteral("Connected")).toBool();
}

bool DeviceInterface::trusted() const
{
    return Object::property(QStringLiteral("Trusted")).toBool();
}

void DeviceInterface::setTrusted(bool trusted)
{
    Object::changeProperty(QStringLiteral("Trusted"), trusted);
}

bool DeviceInterface::blocked() const
{
    return Object::property(QStringLiteral("Blocked")).toBool();
}

void DeviceInterface::setBlocked(bool blocked)
{
    Object::changeProperty(QStringLiteral("Blocked"), blocked);
}

QDBusObjectPath DeviceInterface::adapter() const
{
    return Object::property(QStringLiteral("Adapter")).value<QDBusObjectPath>();
}

bool DeviceInterface::legacyPairing() const
{
    return Object::property(QStringLiteral("LegacyPairing")).toBool();
}

qint16 DeviceInterface::rssi() const
{
    return Object::property(QStringLiteral("RSSI")).toInt();
}

QStringList DeviceInterface::uuids() const
{
    return Object::property(QStringLiteral("UUIDs")).toStringList();
}

QString DeviceInterface::modalias() const
{
    return Object::property(QStringLiteral("Modalias")).toString();
}

void DeviceInterface::Connect()
{
    Object::changeProperty(QStringLiteral("Connected"), true);
}

void DeviceInterface::Disconnect()
{
    Object::changeProperty(QStringLiteral("Connected"), false);
}

void DeviceInterface::ConnectProfile(const QString &uuid)
{
    Q_UNUSED(uuid)

    Q_UNIMPLEMENTED();
}

void DeviceInterface::DisconnectProfile(const QString &uuid)
{
    Q_UNUSED(uuid)

    Q_UNIMPLEMENTED();
}

void DeviceInterface::Pair()
{
    Q_UNIMPLEMENTED();
}

void DeviceInterface::CancelPairing()
{
    Q_UNIMPLEMENTED();
}
