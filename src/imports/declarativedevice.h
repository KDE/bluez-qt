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

#ifndef DECLARATIVEDEVICE_H
#define DECLARATIVEDEVICE_H

#include "device.h"

class DeclarativeAdapter;
class DeclarativeInput;
class DeclarativeMediaPlayer;

class DeclarativeDevice : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString ubi READ ubi CONSTANT)
    Q_PROPERTY(QString address READ address CONSTANT)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString friendlyName READ friendlyName NOTIFY friendlyNameChanged)
    Q_PROPERTY(QString remoteName READ remoteName NOTIFY remoteNameChanged)
    Q_PROPERTY(quint32 deviceClass READ deviceClass NOTIFY deviceClassChanged)
    Q_PROPERTY(BluezQt::Device::Type type READ type NOTIFY typeChanged)
    Q_PROPERTY(quint16 appearance READ appearance NOTIFY appearanceChanged)
    Q_PROPERTY(QString icon READ icon NOTIFY iconChanged)
    Q_PROPERTY(bool paired READ isPaired NOTIFY pairedChanged)
    Q_PROPERTY(bool trusted READ isTrusted WRITE setTrusted NOTIFY trustedChanged)
    Q_PROPERTY(bool blocked READ isBlocked WRITE setBlocked NOTIFY blockedChanged)
    Q_PROPERTY(bool legacyPairing READ hasLegacyPairing NOTIFY legacyPairingChanged)
    Q_PROPERTY(qint16 rssi READ rssi NOTIFY rssiChanged)
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)
    Q_PROPERTY(QStringList uuids READ uuids NOTIFY uuidsChanged)
    Q_PROPERTY(QString modalias READ modalias NOTIFY modaliasChanged)
    Q_PROPERTY(DeclarativeInput* input READ input NOTIFY inputChanged)
    Q_PROPERTY(DeclarativeMediaPlayer* mediaPlayer READ mediaPlayer NOTIFY mediaPlayerChanged)
    Q_PROPERTY(DeclarativeAdapter* adapter READ adapter CONSTANT)

public:
    explicit DeclarativeDevice(BluezQt::DevicePtr device, DeclarativeAdapter *adapter);

    QString ubi() const;

    QString address() const;

    QString name() const;
    void setName(const QString &name);

    QString friendlyName() const;

    QString remoteName() const;

    quint32 deviceClass() const;

    BluezQt::Device::Type type() const;

    quint16 appearance() const;

    QString icon() const;

    bool isPaired() const;

    bool isTrusted() const;
    void setTrusted(bool trusted);

    bool isBlocked() const;
    void setBlocked(bool blocked);

    bool hasLegacyPairing() const;

    qint16 rssi() const;

    bool isConnected() const;

    QStringList uuids() const;

    QString modalias() const;

    DeclarativeInput *input() const;

    DeclarativeMediaPlayer *mediaPlayer() const;

    DeclarativeAdapter *adapter() const;

public Q_SLOTS:
    BluezQt::PendingCall *connectToDevice();
    BluezQt::PendingCall *disconnectFromDevice();
    BluezQt::PendingCall *connectProfile(const QString &uuid);
    BluezQt::PendingCall *disconnectProfile(const QString &uuid);
    BluezQt::PendingCall *pair();
    BluezQt::PendingCall *cancelPairing();

Q_SIGNALS:
    void deviceRemoved(DeclarativeDevice *device);
    void deviceChanged(DeclarativeDevice *device);
    void nameChanged(const QString &name);
    void friendlyNameChanged(const QString &friendlyName);
    void remoteNameChanged(const QString &remoteName);
    void deviceClassChanged(quint32 deviceClass);
    void typeChanged(BluezQt::Device::Type type);
    void appearanceChanged(quint16 appearance);
    void iconChanged(const QString &icon);
    void pairedChanged(bool paired);
    void trustedChanged(bool trusted);
    void blockedChanged(bool blocked);
    void legacyPairingChanged(bool legacyPairing);
    void rssiChanged(qint16 rssi);
    void connectedChanged(bool connected);
    void uuidsChanged(const QStringList &uuids);
    void modaliasChanged(const QString &modalias);
    void inputChanged(DeclarativeInput *input);
    void mediaPlayerChanged(DeclarativeMediaPlayer *mediaPlayer);

private:
    void updateInput();
    void updateMediaPlayer();

    BluezQt::DevicePtr m_device;
    DeclarativeAdapter *m_adapter;
    DeclarativeInput *m_input;
    DeclarativeMediaPlayer *m_mediaPlayer;
};

#endif // DECLARATIVEDEVICE_H
