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

#ifndef DEVICEINTERFACE_H
#define DEVICEINTERFACE_H

#include "object.h"

#include <QStringList>
#include <QDBusAbstractAdaptor>

class QDBusMessage;
class QDBusObjectPath;

class DeviceObject : public QObject
{
public:
    explicit DeviceObject(const QDBusObjectPath &path, QObject *parent = Q_NULLPTR);
};

class DeviceInterface : public QDBusAbstractAdaptor, public Object
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.bluez.Device1")
    Q_PROPERTY(QString Address READ address)
    Q_PROPERTY(QString Name READ name)
    Q_PROPERTY(QString Alias READ alias WRITE setAlias)
    Q_PROPERTY(QString Icon READ icon)
    Q_PROPERTY(quint32 Class READ deviceClass)
    Q_PROPERTY(quint16 Appearance READ appearance)
    Q_PROPERTY(bool Paired READ paired)
    Q_PROPERTY(bool Connected READ connected)
    Q_PROPERTY(bool Trusted READ trusted WRITE setTrusted)
    Q_PROPERTY(bool Blocked READ blocked WRITE setBlocked)
    Q_PROPERTY(QDBusObjectPath Adapter READ adapter)
    Q_PROPERTY(bool LegacyPairing READ legacyPairing)
    Q_PROPERTY(qint16 RSSI READ rssi)
    Q_PROPERTY(QStringList UUIDs READ uuids)
    Q_PROPERTY(QString Modalias READ modalias)

public:
    explicit DeviceInterface(const QDBusObjectPath &path, const QVariantMap &properties, QObject *parent = Q_NULLPTR);

    QString address() const;

    QString name() const;

    QString alias() const;
    void setAlias(const QString &alias);

    QString icon() const;

    quint32 deviceClass() const;

    quint16 appearance() const;

    bool paired() const;

    bool connected() const;

    bool trusted() const;
    void setTrusted(bool trusted);

    bool blocked() const;
    void setBlocked(bool blocked);

    QDBusObjectPath adapter() const;

    bool legacyPairing() const;

    qint16 rssi() const;

    QStringList uuids() const;

    QString modalias() const;

public Q_SLOTS:
    void Connect();
    void Disconnect();
    void ConnectProfile(const QString &uuid, const QDBusMessage &msg);
    void DisconnectProfile(const QString &uuid, const QDBusMessage &msg);
    void Pair();
    void CancelPairing();

private:
    void connectMediaPlayer();
    void disconnectMediaPlayer();

    QStringList m_connectedUuids;
    Object *m_mediaPlayer;
};

#endif // DEVICEINTERFACE_H
