/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * Copyright (C) 2015 David Rosca <nowrep@gmail.com>
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

#include "declarativedevice.h"
#include "declarativeadapter.h"
#include "declarativeinput.h"
#include "declarativemediaplayer.h"

#include <QStringList>

DeclarativeDevice::DeclarativeDevice(BluezQt::DevicePtr device, DeclarativeAdapter *adapter)
    : QObject(adapter)
    , m_device(device)
    , m_adapter(adapter)
    , m_input(0)
    , m_mediaPlayer(0)
{
    connect(m_device.data(), &BluezQt::Device::nameChanged, this, &DeclarativeDevice::nameChanged);
    connect(m_device.data(), &BluezQt::Device::friendlyNameChanged, this, &DeclarativeDevice::friendlyNameChanged);
    connect(m_device.data(), &BluezQt::Device::remoteNameChanged, this, &DeclarativeDevice::remoteNameChanged);
    connect(m_device.data(), &BluezQt::Device::deviceClassChanged, this, &DeclarativeDevice::deviceClassChanged);
    connect(m_device.data(), &BluezQt::Device::typeChanged, this, &DeclarativeDevice::typeChanged);
    connect(m_device.data(), &BluezQt::Device::appearanceChanged, this, &DeclarativeDevice::appearanceChanged);
    connect(m_device.data(), &BluezQt::Device::iconChanged, this, &DeclarativeDevice::iconChanged);
    connect(m_device.data(), &BluezQt::Device::pairedChanged, this, &DeclarativeDevice::pairedChanged);
    connect(m_device.data(), &BluezQt::Device::trustedChanged, this, &DeclarativeDevice::trustedChanged);
    connect(m_device.data(), &BluezQt::Device::blockedChanged, this, &DeclarativeDevice::blockedChanged);
    connect(m_device.data(), &BluezQt::Device::legacyPairingChanged, this, &DeclarativeDevice::legacyPairingChanged);
    connect(m_device.data(), &BluezQt::Device::rssiChanged, this, &DeclarativeDevice::rssiChanged);
    connect(m_device.data(), &BluezQt::Device::connectedChanged, this, &DeclarativeDevice::connectedChanged);
    connect(m_device.data(), &BluezQt::Device::uuidsChanged, this, &DeclarativeDevice::uuidsChanged);
    connect(m_device.data(), &BluezQt::Device::modaliasChanged, this, &DeclarativeDevice::modaliasChanged);
    connect(m_device.data(), &BluezQt::Device::mediaPlayerChanged, this, &DeclarativeDevice::updateMediaPlayer);
    connect(m_device.data(), &BluezQt::Device::inputChanged, this, &DeclarativeDevice::updateInput);

    connect(m_device.data(), &BluezQt::Device::deviceRemoved, this, [this]() {
        Q_EMIT deviceRemoved(this);
    });

    connect(m_device.data(), &BluezQt::Device::deviceChanged, this, [this]() {
        Q_EMIT deviceChanged(this);
    });

    updateInput();
    updateMediaPlayer();
}

QString DeclarativeDevice::ubi() const
{
    return m_device->ubi();
}

QString DeclarativeDevice::address() const
{
    return m_device->address();
}

QString DeclarativeDevice::name() const
{
    return m_device->name();
}

void DeclarativeDevice::setName(const QString &name)
{
    m_device->setName(name);
}

QString DeclarativeDevice::friendlyName() const
{
    return m_device->friendlyName();
}

QString DeclarativeDevice::remoteName() const
{
    return m_device->remoteName();
}

quint32 DeclarativeDevice::deviceClass() const
{
    return m_device->deviceClass();
}

BluezQt::Device::Type DeclarativeDevice::type() const
{
    return m_device->type();
}

quint16 DeclarativeDevice::appearance() const
{
    return m_device->appearance();
}

QString DeclarativeDevice::icon() const
{
    return m_device->icon();
}

bool DeclarativeDevice::isPaired() const
{
    return m_device->isPaired();
}

bool DeclarativeDevice::isTrusted() const
{
    return m_device->isTrusted();
}

void DeclarativeDevice::setTrusted(bool trusted)
{
    m_device->setTrusted(trusted);
}

bool DeclarativeDevice::isBlocked() const
{
    return m_device->isBlocked();
}

void DeclarativeDevice::setBlocked(bool blocked)
{
    m_device->setBlocked(blocked);
}

bool DeclarativeDevice::hasLegacyPairing() const
{
    return m_device->hasLegacyPairing();
}

qint16 DeclarativeDevice::rssi() const
{
    return m_device->rssi();
}

bool DeclarativeDevice::isConnected() const
{
    return m_device->isConnected();
}

QStringList DeclarativeDevice::uuids() const
{
    return m_device->uuids();
}

QString DeclarativeDevice::modalias() const
{
    return m_device->modalias();
}

DeclarativeInput *DeclarativeDevice::input() const
{
    return m_input;
}

DeclarativeMediaPlayer *DeclarativeDevice::mediaPlayer() const
{
    return m_mediaPlayer;
}

DeclarativeAdapter *DeclarativeDevice::adapter() const
{
    return m_adapter;
}

BluezQt::PendingCall *DeclarativeDevice::connectToDevice()
{
    return m_device->connectToDevice();
}

BluezQt::PendingCall *DeclarativeDevice::disconnectFromDevice()
{
    return m_device->disconnectFromDevice();
}

BluezQt::PendingCall *DeclarativeDevice::connectProfile(const QString &uuid)
{
    return m_device->connectProfile(uuid);
}

BluezQt::PendingCall *DeclarativeDevice::disconnectProfile(const QString &uuid)
{
    return m_device->disconnectProfile(uuid);
}

BluezQt::PendingCall *DeclarativeDevice::pair()
{
    return m_device->pair();
}

BluezQt::PendingCall *DeclarativeDevice::cancelPairing()
{
    return m_device->cancelPairing();
}

void DeclarativeDevice::updateInput()
{
    if (m_input) {
        m_input->deleteLater();
        m_input = Q_NULLPTR;
    }

    if (m_device->input()) {
        m_input = new DeclarativeInput(m_device->input(), this);
    }

    Q_EMIT inputChanged(m_input);
}

void DeclarativeDevice::updateMediaPlayer()
{
    if (m_mediaPlayer) {
        m_mediaPlayer->deleteLater();
        m_mediaPlayer = Q_NULLPTR;
    }

    if (m_device->mediaPlayer()) {
        m_mediaPlayer = new DeclarativeMediaPlayer(m_device->mediaPlayer(), this);
    }

    Q_EMIT mediaPlayerChanged(m_mediaPlayer);
}
