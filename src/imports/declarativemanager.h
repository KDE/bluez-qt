/*
 * BluezQt - Asynchronous Bluez wrapper library
 *
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

#ifndef DECLARATIVEMANAGER_H
#define DECLARATIVEMANAGER_H

#include "manager.h"

#include <QHash>
#include <QQmlListProperty>

class DeclarativeAdapter;

class DeclarativeManager : public BluezQt::Manager
{
    Q_OBJECT
    Q_PROPERTY(DeclarativeAdapter* usableAdapter READ usableAdapter NOTIFY usableAdapterChanged)
    Q_PROPERTY(QQmlListProperty<DeclarativeAdapter> adapters READ declarativeAdapters NOTIFY adaptersChanged)
    Q_PROPERTY(QQmlListProperty<BluezQt::Device> devices READ declarativeDevices NOTIFY devicesChanged)

public:
    DeclarativeManager(QObject *parent = 0);

    DeclarativeAdapter *usableAdapter() const;
    QQmlListProperty<DeclarativeAdapter> declarativeAdapters();
    QQmlListProperty<BluezQt::Device> declarativeDevices();

    QHash<QString, DeclarativeAdapter*> m_adapters;

    static DeclarativeManager *self();

public Q_SLOTS:
    DeclarativeAdapter *adapterForDevice(BluezQt::Device *device) const;
    DeclarativeAdapter *adapterForAddress(const QString &address) const;
    DeclarativeAdapter *adapterForUbi(const QString &ubi) const;
    BluezQt::Device *deviceForAddress(const QString &address) const;
    BluezQt::Device *deviceForUbi(const QString &ubi) const;

Q_SIGNALS:
    void initialized();
    void initializeError(const QString &errorText);
    void adapterAdded(DeclarativeAdapter *adapter);
    void adapterRemoved(DeclarativeAdapter *adapter);
    void adapterChanged(DeclarativeAdapter *adapter);
    void deviceAdded(BluezQt::Device *device);
    void deviceRemoved(BluezQt::Device *device);
    void deviceChanged(BluezQt::Device *device);
    void usableAdapterChanged(DeclarativeAdapter *adapter);

    void adaptersChanged(QQmlListProperty<DeclarativeAdapter> adapters);
    void devicesChanged(QQmlListProperty<BluezQt::Device> devices);

private Q_SLOTS:
    void initJobResult(BluezQt::InitManagerJob *job);

    void slotAdapterAdded(BluezQt::AdapterPtr adapter);
    void slotAdapterRemoved(BluezQt::AdapterPtr adapter);
    void slotDeviceAdded(BluezQt::DevicePtr device);
    void slotDeviceRemoved(BluezQt::DevicePtr device);
    void slotUsableAdapterChanged(BluezQt::AdapterPtr adapter);

private:
    DeclarativeAdapter *declarativeAdapterFromPtr(BluezQt::AdapterPtr ptr) const;
};

#endif // DECLARATIVEMANAGER_H
