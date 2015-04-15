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

#include "declarativedevicesmodel.h"
#include "declarativemanager.h"
#include "declarativeadapter.h"
#include "declarativedevice.h"
#include "declarativemediaplayer.h"

DeclarativeDevicesModel::DeclarativeDevicesModel(QObject *parent)
    : QSortFilterProxyModel(parent)
    , m_manager(0)
    , m_model(0)
{
}

DeclarativeManager *DeclarativeDevicesModel::manager() const
{
    return m_manager;
}

void DeclarativeDevicesModel::setManager(DeclarativeManager *manager)
{
    m_manager = manager;
    m_model = new BluezQt::DevicesModel(m_manager, this);
    setSourceModel(m_model);
}

QHash<int, QByteArray> DeclarativeDevicesModel::roleNames() const
{
    QHash<int, QByteArray> roles = QSortFilterProxyModel::roleNames();

    roles[DeviceRole] = QByteArrayLiteral("Device");
    roles[AdapterRole] = QByteArrayLiteral("Adapter");
    roles[MediaPlayerRole] = QByteArrayLiteral("MediaPlayer");

    return roles;
}

QVariant DeclarativeDevicesModel::data(const QModelIndex &index, int role) const
{
    if (!m_model) {
        return QSortFilterProxyModel::data(index, role);
    }

    BluezQt::DevicePtr dev = m_model->device(mapToSource(index));
    if (!dev) {
        return QSortFilterProxyModel::data(index, role);
    }

    switch (role) {
    case DeviceRole:
        return QVariant::fromValue(m_manager->declarativeDeviceFromPtr(dev));
    case AdapterRole:
        return QVariant::fromValue(m_manager->declarativeAdapterFromPtr(dev->adapter()));
    case MediaPlayerRole:
        if (DeclarativeDevice *device = m_manager->declarativeDeviceFromPtr(dev)) {
            return QVariant::fromValue(device->mediaPlayer());
        }
        // fallthrough
    default:
        return QSortFilterProxyModel::data(index, role);
    }
}
