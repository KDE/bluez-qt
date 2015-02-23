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

#include "adapter.h"

#include <QQmlListProperty>

class DeclarativeAdapter : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString ubi READ ubi)
    Q_PROPERTY(QString address READ address)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString alias READ alias WRITE setAlias NOTIFY aliasChanged)
    Q_PROPERTY(quint32 adapterClass READ adapterClass NOTIFY adapterClassChanged)
    Q_PROPERTY(bool powered READ isPowered WRITE setPowered NOTIFY poweredChanged)
    Q_PROPERTY(bool discoverable READ isDiscoverable WRITE setDiscoverable NOTIFY discoverableChanged)
    Q_PROPERTY(quint32 discoverableTimeout READ discoverableTimeout WRITE setDiscoverableTimeout NOTIFY discoverableTimeoutChanged)
    Q_PROPERTY(bool pairable READ isPairable WRITE setPairable NOTIFY pairableChanged)
    Q_PROPERTY(quint32 pairableTimeout READ pairableTimeout WRITE setPairableTimeout NOTIFY pairableTimeoutChanged)
    Q_PROPERTY(bool discovering READ isDiscovering NOTIFY discoveringChanged)
    Q_PROPERTY(QStringList uuids READ uuids NOTIFY uuidsChanged)
    Q_PROPERTY(QString modalias READ modalias NOTIFY modaliasChanged)
    Q_PROPERTY(QQmlListProperty<BluezQt::Device> devices READ devices NOTIFY devicesChanged)

public:
    explicit DeclarativeAdapter(BluezQt::AdapterPtr adapter, QObject *parent = 0);

    QString ubi() const;

    QString address() const;

    QString name() const;

    QString alias() const;
    BluezQt::PendingCall *setAlias(const QString &alias);

    quint32 adapterClass() const;

    bool isPowered() const;
    BluezQt::PendingCall *setPowered(bool powered);

    bool isDiscoverable() const;
    BluezQt::PendingCall *setDiscoverable(bool discoverable);

    quint32 discoverableTimeout() const;
    BluezQt::PendingCall *setDiscoverableTimeout(quint32 timeout);

    bool isPairable() const;
    BluezQt::PendingCall *setPairable(bool pairable);

    quint32 pairableTimeout() const;
    BluezQt::PendingCall *setPairableTimeout(quint32 timeout);

    bool isDiscovering();

    QStringList uuids() const;

    QString modalias() const;

    QQmlListProperty<BluezQt::Device> devices();

    BluezQt::AdapterPtr m_adapter;

public Q_SLOTS:
    BluezQt::Device *deviceForAddress(const QString &address) const;
    BluezQt::PendingCall *startDiscovery();
    BluezQt::PendingCall *stopDiscovery();
    BluezQt::PendingCall *removeDevice(BluezQt::Device *device);

Q_SIGNALS:
    void adapterChanged(DeclarativeAdapter *adapter);
    void nameChanged(const QString &name);
    void aliasChanged(const QString &name);
    void adapterClassChanged(quint32 adapterClass);
    void poweredChanged(bool powered);
    void discoverableChanged(bool discoverable);
    void discoverableTimeoutChanged(quint32 timeout);
    void pairableChanged(bool pairable);
    void pairableTimeoutChanged(quint32 timeout);
    void discoveringChanged(bool discovering);
    void uuidsChanged(const QStringList &uuids);
    void modaliasChanged(const QString &modalias);
    void deviceFound(BluezQt::Device *device);
    void deviceRemoved(BluezQt::Device *device);
    void deviceChanged(BluezQt::Device *device);

    void devicesChanged(QQmlListProperty<BluezQt::Device> devices);

};

#endif // DECLARATIVEADAPTER_H
