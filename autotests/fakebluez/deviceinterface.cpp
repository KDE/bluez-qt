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
#include "objectmanager.h"
#include "mediaplayerinterface.h"
#include "inputinterface.h"

#include <QDBusMessage>
#include <QDBusArgument>
#include <QDBusConnection>

static const QLatin1String MediaPlayerUuid ("0000110E-0000-1000-8000-00805F9B34FB");

// DeviceObject
DeviceObject::DeviceObject(const QDBusObjectPath &path, QObject *parent)
    : QObject(parent)
{
    QDBusConnection::sessionBus().registerObject(path.path(), this);
}

// DeviceInterface
DeviceInterface::DeviceInterface(const QDBusObjectPath &path, const QVariantMap &properties, QObject *parent)
    : QDBusAbstractAdaptor(parent)
    , m_mediaPlayer(0)
{
    setPath(path);
    setObjectParent(parent);
    setProperties(properties);
    setName(QStringLiteral("org.bluez.Device1"));

    // Alias needs special handling
    setAlias(properties.value(QStringLiteral("Alias")).toString());

    // Create Input1
    if (properties.contains(QStringLiteral("Input"))) {
        const QVariantMap &inputProps = qdbus_cast<QVariantMap>(properties.value(QStringLiteral("Input")));
        InputInterface *input = new InputInterface(path, inputProps, parent);

        ObjectManager *manager = ObjectManager::self();
        manager->addObject(input);

        QVariantMap props = properties;
        props.remove(QStringLiteral("Input"));
        setProperties(props);
    }
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
    if (uuids().contains(MediaPlayerUuid)) {
        connectMediaPlayer();
    }

    Object::changeProperty(QStringLiteral("Connected"), true);
}

void DeviceInterface::Disconnect()
{
    if (uuids().contains(MediaPlayerUuid)) {
        disconnectMediaPlayer();
    }

    Object::changeProperty(QStringLiteral("Connected"), false);
}

void DeviceInterface::ConnectProfile(const QString &uuid, const QDBusMessage &msg)
{
    if (!uuids().contains(uuid)) {
        QDBusMessage error = msg.createErrorReply(QStringLiteral("org.bluez.Error.DoesNotExist"), QStringLiteral("Profile UUID not supported"));
        QDBusConnection::sessionBus().send(error);
        return;
    }

    if (m_connectedUuids.contains(uuid)) {
        QDBusMessage error = msg.createErrorReply(QStringLiteral("org.bluez.Error.AlreadyConnected"), QStringLiteral("Profile already connected"));
        QDBusConnection::sessionBus().send(error);
        return;
    }

    if (uuid == MediaPlayerUuid) {
        connectMediaPlayer();
    } else {
        Q_UNIMPLEMENTED();
    }

    Object::changeProperty(QStringLiteral("Connected"), true);
}

void DeviceInterface::DisconnectProfile(const QString &uuid, const QDBusMessage &msg)
{
    if (!uuids().contains(uuid)) {
        QDBusMessage error = msg.createErrorReply(QStringLiteral("org.bluez.Error.DoesNotExist"), QStringLiteral("Profile UUID not supported"));
        QDBusConnection::sessionBus().send(error);
        return;
    }

    if (!m_connectedUuids.contains(uuid)) {
        QDBusMessage error = msg.createErrorReply(QStringLiteral("org.bluez.Error.NotConnected"), QStringLiteral("Profile not connected"));
        QDBusConnection::sessionBus().send(error);
        return;
    }

    if (uuid == MediaPlayerUuid) {
        disconnectMediaPlayer();
    } else {
        Q_UNIMPLEMENTED();
    }

    Object::changeProperty(QStringLiteral("Connected"), false);
}

void DeviceInterface::Pair()
{
    Q_UNIMPLEMENTED();
}

void DeviceInterface::CancelPairing()
{
    Q_UNIMPLEMENTED();
}

void DeviceInterface::connectMediaPlayer()
{
    const QVariantMap &properties = qdbus_cast<QVariantMap>(Object::property(QStringLiteral("MediaPlayer")));
    const QDBusObjectPath &path = properties.value(QStringLiteral("Path")).value<QDBusObjectPath>();
    QVariantMap props = properties;
    props.remove(QStringLiteral("Path"));

    MediaPlayerObject *mediaPlayerObj = new MediaPlayerObject(path);
    m_mediaPlayer = new MediaPlayerInterface(path, props, mediaPlayerObj);

    ObjectManager *manager = ObjectManager::self();
    manager->addObject(m_mediaPlayer);
    manager->addAutoDeleteObject(mediaPlayerObj);

    m_connectedUuids.append(MediaPlayerUuid);
}

void DeviceInterface::disconnectMediaPlayer()
{
    ObjectManager *manager = ObjectManager::self();
    manager->removeObject(m_mediaPlayer);
    m_connectedUuids.removeOne(MediaPlayerUuid);

    m_mediaPlayer = 0;
}
