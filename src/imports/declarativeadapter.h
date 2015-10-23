/*
 * BluezQt - Asynchronous Bluez wrapper library
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

#ifndef DECLARATIVEADAPTER_H
#define DECLARATIVEADAPTER_H

#include <QQmlListProperty>

#include "adapter.h"

class DeclarativeDevice;

class DeclarativeAdapter : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString ubi READ ubi CONSTANT)
    Q_PROPERTY(QString address READ address CONSTANT)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString systemName READ systemName NOTIFY systemNameChanged)
    Q_PROPERTY(quint32 adapterClass READ adapterClass NOTIFY adapterClassChanged)
    Q_PROPERTY(bool powered READ isPowered WRITE setPowered NOTIFY poweredChanged)
    Q_PROPERTY(bool discoverable READ isDiscoverable WRITE setDiscoverable NOTIFY discoverableChanged)
    Q_PROPERTY(quint32 discoverableTimeout READ discoverableTimeout WRITE setDiscoverableTimeout NOTIFY discoverableTimeoutChanged)
    Q_PROPERTY(bool pairable READ isPairable WRITE setPairable NOTIFY pairableChanged)
    Q_PROPERTY(quint32 pairableTimeout READ pairableTimeout WRITE setPairableTimeout NOTIFY pairableTimeoutChanged)
    Q_PROPERTY(bool discovering READ isDiscovering NOTIFY discoveringChanged)
    Q_PROPERTY(QStringList uuids READ uuids NOTIFY uuidsChanged)
    Q_PROPERTY(QString modalias READ modalias NOTIFY modaliasChanged)
    Q_PROPERTY(QQmlListProperty<DeclarativeDevice> devices READ devices NOTIFY devicesChanged)

public:
    explicit DeclarativeAdapter(BluezQt::AdapterPtr adapter, QObject *parent = Q_NULLPTR);

    QString ubi() const;

    QString address() const;

    QString name() const;
    void setName(const QString &name);

    QString systemName() const;

    quint32 adapterClass() const;

    bool isPowered() const;
    void setPowered(bool powered);

    bool isDiscoverable() const;
    void setDiscoverable(bool discoverable);

    quint32 discoverableTimeout() const;
    void setDiscoverableTimeout(quint32 timeout);

    bool isPairable() const;
    void setPairable(bool pairable);

    quint32 pairableTimeout() const;
    void setPairableTimeout(quint32 timeout);

    bool isDiscovering();

    QStringList uuids() const;

    QString modalias() const;

    QQmlListProperty<DeclarativeDevice> devices();

    BluezQt::AdapterPtr m_adapter;
    QHash<QString, DeclarativeDevice*> m_devices;

public Q_SLOTS:
    DeclarativeDevice *deviceForAddress(const QString &address) const;
    BluezQt::PendingCall *startDiscovery();
    BluezQt::PendingCall *stopDiscovery();
    BluezQt::PendingCall *removeDevice(DeclarativeDevice *device);

Q_SIGNALS:
    void adapterRemoved(DeclarativeAdapter *adapter);
    void adapterChanged(DeclarativeAdapter *adapter);
    void nameChanged(const QString &name);
    void systemNameChanged(const QString &name);
    void adapterClassChanged(quint32 adapterClass);
    void poweredChanged(bool powered);
    void discoverableChanged(bool discoverable);
    void discoverableTimeoutChanged(quint32 timeout);
    void pairableChanged(bool pairable);
    void pairableTimeoutChanged(quint32 timeout);
    void discoveringChanged(bool discovering);
    void uuidsChanged(const QStringList &uuids);
    void modaliasChanged(const QString &modalias);
    void deviceFound(DeclarativeDevice *device);
    void deviceRemoved(DeclarativeDevice *device);
    void deviceChanged(DeclarativeDevice *device);

    void devicesChanged(QQmlListProperty<DeclarativeDevice> devices);

private Q_SLOTS:
    void slotDeviceAdded(BluezQt::DevicePtr device);
    void slotDeviceRemoved(BluezQt::DevicePtr device);

private:
    DeclarativeDevice *declarativeDeviceFromPtr(BluezQt::DevicePtr ptr) const;
};

#endif // DECLARATIVEADAPTER_H
