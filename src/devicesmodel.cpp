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

#include "devicesmodel.h"
#include "manager.h"
#include "adapter.h"
#include "device.h"

namespace BluezQt
{

class DevicesModelPrivate : public QObject
{
public:
    explicit DevicesModelPrivate(DevicesModel *q);

    void init();

    void adapterAdded(AdapterPtr adapter);
    void deviceFound(DevicePtr device);
    void deviceRemoved(DevicePtr device);
    void deviceChanged(DevicePtr device);

    DevicesModel *q;
    Manager *m_manager;
    QList<DevicePtr> m_devices;
};

DevicesModelPrivate::DevicesModelPrivate(DevicesModel *q)
    : QObject(q)
    , q(q)
    , m_manager(0)
{
}

void DevicesModelPrivate::init()
{
    m_devices = m_manager->devices();

    Q_FOREACH (AdapterPtr adapter, m_manager->adapters()) {
        adapterAdded(adapter);
    }

    connect(m_manager, &Manager::adapterAdded, this, &DevicesModelPrivate::adapterAdded);
}

void DevicesModelPrivate::adapterAdded(AdapterPtr adapter)
{
    connect(adapter.data(), &Adapter::deviceFound, this, &DevicesModelPrivate::deviceFound);
    connect(adapter.data(), &Adapter::deviceRemoved, this, &DevicesModelPrivate::deviceRemoved);
    connect(adapter.data(), &Adapter::deviceChanged, this, &DevicesModelPrivate::deviceChanged);
}

void DevicesModelPrivate::deviceFound(DevicePtr device)
{
    q->beginInsertRows(QModelIndex(), m_devices.size(), m_devices.size());
    m_devices.append(device);
    q->endInsertRows();

    Q_EMIT q->deviceFound(device);
}

void DevicesModelPrivate::deviceRemoved(DevicePtr device)
{
    int offset = m_devices.indexOf(device);
    Q_ASSERT(offset >= 0);

    q->beginRemoveRows(QModelIndex(), offset, offset);
    m_devices.removeAt(offset);
    q->endRemoveRows();

    Q_EMIT q->deviceRemoved(device);
}

void DevicesModelPrivate::deviceChanged(DevicePtr device)
{
    int offset = m_devices.indexOf(device);
    Q_ASSERT(offset >= 0);

    QModelIndex idx = q->createIndex(offset, 0);
    Q_EMIT q->dataChanged(idx, idx);

    Q_EMIT q->deviceChanged(device);
}

DevicesModel::DevicesModel(Manager *manager, QObject *parent)
    : QAbstractListModel(parent)
    , d(new DevicesModelPrivate(this))
{
    d->m_manager = manager;
    d->init();
}

QHash<int, QByteArray> DevicesModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();

    roles[UbiRole] = QByteArrayLiteral("Ubi");
    roles[AddressRole] = QByteArrayLiteral("Address");
    roles[NameRole] = QByteArrayLiteral("Name");
    roles[FriendlyNameRole] = QByteArrayLiteral("FriendlyName");
    roles[RemoteNameRole] = QByteArrayLiteral("RemoteName");
    roles[DeviceClassRole] = QByteArrayLiteral("DeviceClass");
    roles[DeviceTypeRole] = QByteArrayLiteral("DeviceType");
    roles[AppearanceRole] = QByteArrayLiteral("Appearance");
    roles[IconRole] = QByteArrayLiteral("Icon");
    roles[PairedRole] = QByteArrayLiteral("Paired");
    roles[TrustedRole] = QByteArrayLiteral("Trusted");
    roles[BlockedRole] = QByteArrayLiteral("Blocked");
    roles[LegacyPairingRole] = QByteArrayLiteral("LegacyPairing");
    roles[RssiRole] = QByteArrayLiteral("Rssi");
    roles[ConnectedRole] = QByteArrayLiteral("Connected");
    roles[UuidsRole] = QByteArrayLiteral("Uuids");
    roles[ModaliasRole] = QByteArrayLiteral("Modalias");

    return roles;
}

int DevicesModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return d->m_devices.size();
}

QVariant DevicesModel::data(const QModelIndex &index, int role) const
{
    DevicePtr dev = device(index);

    switch (role) {
    case Qt::DisplayRole:
        return dev->name();
    case Qt::ToolTipRole:
        return dev->friendlyName();
    case UbiRole:
        return dev->ubi();
    case AddressRole:
        return dev->address();
    case NameRole:
        return dev->name();
    case FriendlyNameRole:
        return dev->friendlyName();
    case RemoteNameRole:
        return dev->remoteName();
    case DeviceClassRole:
        return dev->deviceClass();
    case DeviceTypeRole:
        return dev->deviceType();
    case AppearanceRole:
        return dev->appearance();
    case IconRole:
        return dev->icon();
    case PairedRole:
        return dev->isPaired();
    case TrustedRole:
        return dev->isTrusted();
    case BlockedRole:
        return dev->isBlocked();
    case LegacyPairingRole:
        return dev->hasLegacyPairing();
    case RssiRole:
        return dev->rssi();
    case ConnectedRole:
        return dev->isConnected();
    case UuidsRole:
        return dev->uuids();
    case ModaliasRole:
        return dev->modalias();
    default:
        return QVariant();
    }
}

QModelIndex DevicesModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }
    return createIndex(row, 0);
}

DevicePtr DevicesModel::device(const QModelIndex &index) const
{
    return d->m_devices.at(index.row());
}

} // namespace BluezQt
