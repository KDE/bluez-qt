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

#ifndef BLUEZQT_ADAPTER_P_H
#define BLUEZQT_ADAPTER_P_H

#include <QObject>
#include <QStringList>

#include "types.h"
#include "bluezadapter1.h"
#include "dbusproperties.h"

namespace BluezQt
{

typedef org::bluez::Adapter1 BluezAdapter;
typedef org::freedesktop::DBus::Properties DBusProperties;

class AdapterPrivate : public QObject
{
    Q_OBJECT

public:
    explicit AdapterPrivate(const QString &path, const QVariantMap &properties);

    void init(const QVariantMap &properties);

    void addDevice(const DevicePtr &device);
    void removeDevice(const DevicePtr &device);

    QDBusPendingReply<> setDBusProperty(const QString &name, const QVariant &value);
    void propertiesChanged(const QString &interface, const QVariantMap &changed, const QStringList &invalidated);

    QWeakPointer<Adapter> q;
    BluezAdapter *m_bluezAdapter;
    DBusProperties *m_dbusProperties;

    QString m_address;
    QString m_name;
    QString m_alias;
    quint32 m_adapterClass;
    bool m_powered;
    bool m_discoverable;
    quint32 m_discoverableTimeout;
    bool m_pairable;
    quint32 m_pairableTimeout;
    bool m_discovering;
    QStringList m_uuids;
    QList<DevicePtr> m_devices;
    QString m_modalias;
};

} // namespace BluezQt

#endif // BLUEZQT_ADAPTER_P_H
