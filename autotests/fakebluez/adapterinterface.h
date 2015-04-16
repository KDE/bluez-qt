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

#ifndef ADAPTERINTERFACE_H
#define ADAPTERINTERFACE_H

#include "object.h"

#include <QDBusAbstractAdaptor>

class QDBusObjectPath;

class AdapterObject : public QObject
{
public:
    explicit AdapterObject(const QDBusObjectPath &path, QObject *parent = Q_NULLPTR);
};

class AdapterInterface : public QDBusAbstractAdaptor, public Object
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.bluez.Adapter1")
    Q_PROPERTY(QString Address READ address)
    Q_PROPERTY(QString Name READ name)
    Q_PROPERTY(QString Alias READ alias WRITE setAlias)
    Q_PROPERTY(quint32 Class READ adapterClass)
    Q_PROPERTY(bool Powered READ powered WRITE setPowered)
    Q_PROPERTY(bool Discoverable READ discoverable WRITE setDiscoverable)
    Q_PROPERTY(bool Pairable READ pairable WRITE setPairable)
    Q_PROPERTY(quint32 PairableTimeout READ pairableTimeout WRITE setPairableTimeout)
    Q_PROPERTY(quint32 DiscoverableTimeout READ discoverableTimeout WRITE setDiscoverableTimeout)
    Q_PROPERTY(bool Discovering READ discovering)
    Q_PROPERTY(QStringList UUIDs READ uuids)
    Q_PROPERTY(QString Modalias READ modalias)

public:
    explicit AdapterInterface(const QDBusObjectPath &path, const QVariantMap &properties, QObject *parent = Q_NULLPTR);

    QString address() const;

    QString name() const;

    QString alias() const;
    void setAlias(const QString &alias);

    quint32 adapterClass() const;

    bool powered() const;
    void setPowered(bool powered);

    bool discoverable() const;
    void setDiscoverable(bool discoverable);

    bool pairable() const;
    void setPairable(bool pairable);

    quint32 pairableTimeout() const;
    void setPairableTimeout(quint32 timeout);

    quint32 discoverableTimeout() const;
    void setDiscoverableTimeout(quint32 timeout);

    bool discovering() const;

    QStringList uuids() const;

    QString modalias() const;

public Q_SLOTS:
    void StartDiscovery();
    void StopDiscovery();
    void RemoveDevice(const QDBusObjectPath &device);

private Q_SLOTS:
    void resetPairable();
    void resetDiscoverable();
};

#endif // ADAPTERINTERFACE_H
