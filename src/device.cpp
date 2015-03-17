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

#include "device.h"
#include "device_p.h"
#include "pendingcall.h"
#include "utils_p.h"

namespace BluezQt
{

Device::Device(const QString &path, const QVariantMap &properties, AdapterPtr adapter)
    : QObject()
    , d(new DevicePrivate(path, properties, adapter))
{
}

Device::~Device()
{
    delete d;
}

DevicePtr Device::toSharedPtr() const
{
    return d->q.toStrongRef();
}

QString Device::ubi() const
{
    return d->m_bluezDevice->path();
}

QString Device::address() const
{
    return d->m_address;
}

QString Device::name() const
{
    return d->m_alias;
}

PendingCall *Device::setName(const QString &name)
{
    return new PendingCall(d->setDBusProperty(QStringLiteral("Alias"), name),
                           PendingCall::ReturnVoid, this);
}

QString Device::friendlyName() const
{
    if (name().isEmpty() || name() == remoteName()) {
        return name();
    }
    if (remoteName().isEmpty()) {
        return name();
    }
    return QString(QStringLiteral("%1 (%2)")).arg(name(), remoteName());
}

QString Device::remoteName() const
{
    return d->m_name;
}

quint32 Device::deviceClass() const
{
    return d->m_deviceClass;
}

Device::DeviceType Device::deviceType() const
{
    return classToType(d->m_deviceClass);
}

quint16 Device::appearance() const
{
    return d->m_appearance;
}

QString Device::icon() const
{
    return d->m_icon.isEmpty() ? QStringLiteral("preferences-system-bluetooth") : d->m_icon;
}

bool Device::isPaired() const
{
    return d->m_paired;
}

bool Device::isTrusted() const
{
    return d->m_trusted;
}

PendingCall *Device::setTrusted(bool trusted)
{
    return new PendingCall(d->setDBusProperty(QStringLiteral("Trusted"), trusted),
                           PendingCall::ReturnVoid, this);
}

bool Device::isBlocked() const
{
    return d->m_blocked;
}

PendingCall *Device::setBlocked(bool blocked)
{
    return new PendingCall(d->setDBusProperty(QStringLiteral("Blocked"), blocked),
                           PendingCall::ReturnVoid, this);
}

bool Device::hasLegacyPairing() const
{
    return d->m_legacyPairing;
}

qint16 Device::rssi() const
{
    return d->m_rssi;
}

bool Device::isConnected() const
{
    return d->m_connected;
}

QStringList Device::uuids() const
{
    return d->m_uuids;
}

QString Device::modalias() const
{
    return d->m_modalias;
}

AdapterPtr Device::adapter() const
{
    return d->m_adapter;
}

QString Device::typeToString(Device::DeviceType type)
{
    switch (type) {
    case Device::Any:
        return QStringLiteral("any");
    case Device::Phone:
        return QStringLiteral("phone");
    case Device::Modem:
        return QStringLiteral("modem");
    case Device::Computer:
        return QStringLiteral("computer");
    case Device::Network:
        return QStringLiteral("network");
    case Device::Headset:
        return QStringLiteral("headset");
    case Device::Headphones:
        return QStringLiteral("headphones");
    case Device::OtherAudio:
        return QStringLiteral("audio");
    case Device::Keyboard:
        return QStringLiteral("keyboard");
    case Device::Mouse:
        return QStringLiteral("mouse");
    case Device::Camera:
        return QStringLiteral("camera");
    case Device::Printer:
        return QStringLiteral("printer");
    case Device::Joypad:
        return QStringLiteral("joypad");
    case Device::Tablet:
        return QStringLiteral("tablet");
    default:
        return QStringLiteral("any");
    }
}

Device::DeviceType Device::stringToType(const QString &typeString)
{
    if (typeString == QLatin1String("any")) {
        return Device::Any;
    } else if (typeString == QLatin1String("phone")) {
        return Device::Phone;
    } else if (typeString == QLatin1String("modem")) {
        return Device::Modem;
    } else if (typeString == QLatin1String("computer")) {
        return Device::Computer;
    } else if (typeString == QLatin1String("network")) {
        return Device::Network;
    } else if (typeString == QLatin1String("headset")) {
        return Device::Headset;
    } else if (typeString == QLatin1String("headphones")) {
        return Device::Headphones;
    } else if (typeString == QLatin1String("audio")) {
        return Device::OtherAudio;
    } else if (typeString == QLatin1String("keyboard")) {
        return Device::Keyboard;
    } else if (typeString == QLatin1String("mouse")) {
        return Device::Mouse;
    } else if (typeString == QLatin1String("camera")) {
        return Device::Camera;
    } else if (typeString == QLatin1String("printer")) {
        return Device::Printer;
    } else if (typeString == QLatin1String("joypad")) {
        return Device::Joypad;
    } else if (typeString == QLatin1String("tablet")) {
        return Device::Tablet;
    }
    return Device::Any;
}

PendingCall *Device::connectDevice()
{
    return new PendingCall(d->m_bluezDevice->Connect(), PendingCall::ReturnVoid, this);
}

PendingCall *Device::disconnectDevice()
{
    return new PendingCall(d->m_bluezDevice->Disconnect(), PendingCall::ReturnVoid, this);
}

PendingCall *Device::connectProfile(const QString &uuid)
{
    return new PendingCall(d->m_bluezDevice->ConnectProfile(uuid), PendingCall::ReturnVoid, this);
}

PendingCall *Device::disconnectProfile(const QString &uuid)
{
    return new PendingCall(d->m_bluezDevice->DisconnectProfile(uuid), PendingCall::ReturnVoid, this);
}

PendingCall *Device::pair()
{
    return new PendingCall(d->m_bluezDevice->Pair(), PendingCall::ReturnVoid, this);
}

PendingCall *Device::cancelPairing()
{
    return new PendingCall(d->m_bluezDevice->CancelPairing(), PendingCall::ReturnVoid, this);
}

} // namespace BluezQt
