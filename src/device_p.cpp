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

#include "device_p.h"
#include "device.h"
#include "adapter.h"
#include "input.h"
#include "input_p.h"
#include "mediaplayer.h"
#include "mediaplayer_p.h"
#include "utils.h"
#include "macros.h"

namespace BluezQt
{

static const qint16 INVALID_RSSI = -32768; // qint16 minimum

DevicePrivate::DevicePrivate(const QString &path, const QVariantMap &properties, const AdapterPtr &adapter)
    : QObject()
    , m_dbusProperties(nullptr)
    , m_deviceClass(0)
    , m_appearance(0)
    , m_paired(false)
    , m_trusted(false)
    , m_blocked(false)
    , m_legacyPairing(false)
    , m_rssi(INVALID_RSSI)
    , m_connected(false)
    , m_adapter(adapter)
{
    m_bluezDevice = new BluezDevice(Strings::orgBluez(), path, DBusConnection::orgBluez(), this);

    init(properties);
}

void DevicePrivate::init(const QVariantMap &properties)
{
    m_dbusProperties = new DBusProperties(Strings::orgBluez(), m_bluezDevice->path(),
                                          DBusConnection::orgBluez(), this);

    // Init properties
    m_address = properties.value(QStringLiteral("Address")).toString();
    m_name = properties.value(QStringLiteral("Name")).toString();
    m_alias = properties.value(QStringLiteral("Alias")).toString();
    m_deviceClass = properties.value(QStringLiteral("Class")).toUInt();
    m_appearance = properties.value(QStringLiteral("Appearance")).toUInt();
    m_icon = properties.value(QStringLiteral("Icon")).toString();
    m_paired = properties.value(QStringLiteral("Paired")).toBool();
    m_trusted = properties.value(QStringLiteral("Trusted")).toBool();
    m_blocked = properties.value(QStringLiteral("Blocked")).toBool();
    m_legacyPairing = properties.value(QStringLiteral("LegacyPairing")).toBool();
    m_rssi = properties.value(QStringLiteral("RSSI")).toInt();
    m_connected = properties.value(QStringLiteral("Connected")).toBool();
    m_uuids = stringListToUpper(properties.value(QStringLiteral("UUIDs")).toStringList());
    m_modalias = properties.value(QStringLiteral("Modalias")).toString();

    if (!m_rssi) {
        m_rssi = INVALID_RSSI;
    }
}

void DevicePrivate::interfacesAdded(const QString &path, const QVariantMapMap &interfaces)
{
    bool changed = false;
    QVariantMapMap::const_iterator it;

    for (it = interfaces.constBegin(); it != interfaces.constEnd(); ++it) {
        if (it.key() == Strings::orgBluezInput1()) {
            m_input = InputPtr(new Input(path, it.value()));
            m_input->d->q = m_input.toWeakRef();
            Q_EMIT q.data()->inputChanged(m_input);
            changed = true;
        } else if (it.key() == Strings::orgBluezMediaPlayer1()) {
            m_mediaPlayer = MediaPlayerPtr(new MediaPlayer(path, it.value()));
            m_mediaPlayer->d->q = m_mediaPlayer.toWeakRef();
            Q_EMIT q.data()->mediaPlayerChanged(m_mediaPlayer);
            changed = true;
        }
    }

    if (changed) {
        Q_EMIT q.data()->deviceChanged(q.toStrongRef());
    }
}

void DevicePrivate::interfacesRemoved(const QString &path, const QStringList &interfaces)
{
    Q_UNUSED(path)
    bool changed = false;

    Q_FOREACH (const QString &interface, interfaces) {
        if (interface == Strings::orgBluezInput1()) {
            m_input.clear();
            Q_EMIT q.data()->inputChanged(m_input);
            changed = true;
        } else if (interface == Strings::orgBluezMediaPlayer1()) {
            m_mediaPlayer.clear();
            Q_EMIT q.data()->mediaPlayerChanged(m_mediaPlayer);
            changed = true;
        }
    }

    if (changed) {
        Q_EMIT q.data()->deviceChanged(q.toStrongRef());
    }
}

QDBusPendingReply<> DevicePrivate::setDBusProperty(const QString &name, const QVariant &value)
{
    return m_dbusProperties->Set(Strings::orgBluezDevice1(), name, QDBusVariant(value));
}

void DevicePrivate::propertiesChanged(const QString &interface, const QVariantMap &changed, const QStringList &invalidated)
{
    if (interface == Strings::orgBluezInput1() && m_input) {
        m_input->d->propertiesChanged(interface, changed, invalidated);
    } else if (interface == Strings::orgBluezMediaPlayer1() && m_mediaPlayer) {
        m_mediaPlayer->d->propertiesChanged(interface, changed, invalidated);
    } else if (interface != Strings::orgBluezDevice1()) {
        return;
    }

    QVariantMap::const_iterator i;
    for (i = changed.constBegin(); i != changed.constEnd(); ++i) {
        const QVariant &value = i.value();
        const QString &property = i.key();

        if (property == QLatin1String("Name")) {
            namePropertyChanged(value.toString());
        } else if (property == QLatin1String("Address")) {
            addressPropertyChanged(value.toString());
        } else if (property == QLatin1String("Alias")) {
            aliasPropertyChanged(value.toString());
        } else if (property == QLatin1String("Class")) {
            classPropertyChanged(value.toUInt());
        } else if (property == QLatin1String("Appearance")) {
            PROPERTY_CHANGED(m_appearance, toUInt, appearanceChanged);
        } else if (property == QLatin1String("Icon")) {
            PROPERTY_CHANGED(m_icon, toString, iconChanged);
        } else if (property == QLatin1String("Paired")) {
            PROPERTY_CHANGED(m_paired, toBool, pairedChanged);
        } else if (property == QLatin1String("Trusted")) {
            PROPERTY_CHANGED(m_trusted, toBool, trustedChanged);
        } else if (property == QLatin1String("Blocked")) {
            PROPERTY_CHANGED(m_blocked, toBool, blockedChanged);
        } else if (property == QLatin1String("LegacyPairing")) {
            PROPERTY_CHANGED(m_legacyPairing, toBool, legacyPairingChanged);
        } else if (property == QLatin1String("RSSI")) {
            PROPERTY_CHANGED(m_rssi, toInt, rssiChanged);
        } else if (property == QLatin1String("Connected")) {
            PROPERTY_CHANGED(m_connected, toBool, connectedChanged);
        } else if (property == QLatin1String("Modalias")) {
            PROPERTY_CHANGED(m_modalias, toString, modaliasChanged);
        } else if (property == QLatin1String("UUIDs")) {
            PROPERTY_CHANGED2(m_uuids, stringListToUpper(value.toStringList()), uuidsChanged);
        }
    }

    Q_FOREACH (const QString &property, invalidated) {
        if (property == QLatin1String("Name")) {
            namePropertyChanged(QString());
        } else if (property == QLatin1String("Class")) {
            classPropertyChanged(0);
        } else if (property == QLatin1String("Appearance")) {
            PROPERTY_INVALIDATED(m_appearance, 0, appearanceChanged);
        } else if (property == QLatin1String("Icon")) {
            PROPERTY_INVALIDATED(m_icon, QString(), iconChanged);
        } else if (property == QLatin1String("RSSI")) {
            PROPERTY_INVALIDATED(m_rssi, INVALID_RSSI, rssiChanged);
        } else if (property == QLatin1String("Modalias")) {
            PROPERTY_INVALIDATED(m_modalias, QString(), modaliasChanged);
        } else if (property == QLatin1String("UUIDs")) {
            PROPERTY_INVALIDATED(m_uuids, QStringList(), uuidsChanged);
        }
    }

    Q_EMIT q.data()->deviceChanged(q.toStrongRef());
}

void DevicePrivate::namePropertyChanged(const QString &value)
{
    if (m_name != value) {
        m_name = value;
        Q_EMIT q.data()->remoteNameChanged(m_name);
        Q_EMIT q.data()->friendlyNameChanged(q.data()->friendlyName());
    }
}

void DevicePrivate::addressPropertyChanged(const QString &value)
{
    if (m_address != value) {
        m_address = value;
        Q_EMIT q.data()->addressChanged(m_address);
    }
}

void DevicePrivate::aliasPropertyChanged(const QString &value)
{
    if (m_alias != value) {
        m_alias = value;
        Q_EMIT q.data()->nameChanged(m_alias);
        Q_EMIT q.data()->friendlyNameChanged(q.data()->friendlyName());
    }
}

void DevicePrivate::classPropertyChanged(quint32 value)
{
    if (m_deviceClass != value) {
        m_deviceClass = value;
        Q_EMIT q.data()->deviceClassChanged(m_deviceClass);
        Q_EMIT q.data()->typeChanged(q.data()->type());
    }
}

} // namespace BluezQt
