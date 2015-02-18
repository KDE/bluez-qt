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

#ifndef DECLARATIVEMANAGER_H
#define DECLARATIVEMANAGER_H

#include "manager.h"

#include <QQmlListProperty>

class DeclarativeManager : public BluezQt::Manager
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<BluezQt::Adapter> adapters READ declarativeAdapters NOTIFY adaptersChanged)
    Q_PROPERTY(QQmlListProperty<BluezQt::Device> devices READ declarativeDevices NOTIFY devicesChanged)

public:
    DeclarativeManager(QObject *parent = 0);

    QQmlListProperty<BluezQt::Adapter> declarativeAdapters();
    QQmlListProperty<BluezQt::Device> declarativeDevices();

    static DeclarativeManager *self();

Q_SIGNALS:
    void initialized();
    void initializeError(const QString &errorText);

    void adaptersChanged(QQmlListProperty<BluezQt::Adapter> adapters);
    void devicesChanged(QQmlListProperty<BluezQt::Device> devices);

private:
    void initJobResult(BluezQt::InitManagerJob *job);

    void slotAdapterAdded(BluezQt::Adapter *adapter);
    void slotAdapterRemoved(BluezQt::Adapter *adapter);
    void slotDeviceAdded(BluezQt::Device *device);
    void slotDeviceRemoved(BluezQt::Device *device);
};

#endif // DECLARATIVEMANAGER_H
