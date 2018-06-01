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

#ifndef BLUEZQT_DEVICE_P_H
#define BLUEZQT_DEVICE_P_H

#include <QObject>
#include <QStringList>

#include "types.h"
#include "bluezdevice1.h"
#include "dbusproperties.h"
#include "bluezqt_dbustypes.h"

namespace BluezQt
{

typedef org::bluez::Device1 BluezDevice;
typedef org::freedesktop::DBus::Properties DBusProperties;

class DevicePrivate : public QObject
{
    Q_OBJECT

public:
    explicit DevicePrivate(const QString &path, const QVariantMap &properties, const AdapterPtr &adapter);

    void init(const QVariantMap &properties);

    void interfacesAdded(const QString &path, const QVariantMapMap &interfaces);
    void interfacesRemoved(const QString &path, const QStringList &interfaces);

    QDBusPendingReply<> setDBusProperty(const QString &name, const QVariant &value);
    void propertiesChanged(const QString &interface, const QVariantMap &changed, const QStringList &invalidated);
    void namePropertyChanged(const QString &value);
    void aliasPropertyChanged(const QString &value);
    void addressPropertyChanged(const QString &value);
    void classPropertyChanged(quint32 value);

    QWeakPointer<Device> q;
    BluezDevice *m_bluezDevice;
    DBusProperties *m_dbusProperties;

    QString m_address;
    QString m_name;
    QString m_alias;
    quint32 m_deviceClass;
    quint16 m_appearance;
    QString m_icon;
    bool m_paired;
    bool m_trusted;
    bool m_blocked;
    bool m_legacyPairing;
    qint16 m_rssi;
    bool m_connected;
    QStringList m_uuids;
    QString m_modalias;
    InputPtr m_input;
    MediaPlayerPtr m_mediaPlayer;
    AdapterPtr m_adapter;
};

} // namespace BluezQt

#endif // BLUEZQT_DEVICE_P_H
