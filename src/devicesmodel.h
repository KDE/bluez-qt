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

#ifndef BLUEZQT_DEVICESMODEL_H
#define BLUEZQT_DEVICESMODEL_H

#include <QAbstractListModel>

#include "types.h"
#include "bluezqt_export.h"

namespace BluezQt
{

class Manager;
class Device;

/**
 * Model of all devices.
 *
 * This class represents a model of all devices.
 *
 * In QML code, this class can only be used after Manager is instantiated.
 */
class BLUEZQT_EXPORT DevicesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    /**
     * Device data roles.
     */
    enum DeviceRoles {
        /** UBI of the device (QString) */
        UbiRole = Qt::UserRole + 100,
        /** Address of the device (QString) */
        AddressRole = Qt::UserRole + 101,
        /** Name of the device (QString) */
        NameRole = Qt::UserRole + 102,
        /** Friendly name of the device (QString) */
        FriendlyNameRole = Qt::UserRole + 103,
        /** Remote name of the device (QString) */
        RemoteNameRole = Qt::UserRole + 104,
        /** Class of the device (quint32) */
        DeviceClassRole = Qt::UserRole + 105,
        /** Type of the device (DeviceType) */
        DeviceTypeRole = Qt::UserRole + 106,
        /** Appearance of the device (quint16) */
        AppearanceRole = Qt::UserRole + 107,
        /** Icon name of the device (QString) */
        IconRole = Qt::UserRole + 108,
        /** Indicates whether the device is paired (bool) */
        PairedRole = Qt::UserRole + 109,
        /** Indicates whether the device is trusted (bool) */
        TrustedRole = Qt::UserRole + 110,
        /** Indicates whether the device is blocked (bool) */
        BlockedRole = Qt::UserRole + 111,
        /** Indicates whether the device has legacy pairing (bool) */
        LegacyPairingRole = Qt::UserRole + 112,
        /** Received Signal Strength Indicator of the device (qint16) */
        RssiRole = Qt::UserRole + 113,
        /** Indicates whether the device is connected (bool) */
        ConnectedRole = Qt::UserRole + 114,
        /** UUIDs of supported services by the device (QStringList) */
        UuidsRole = Qt::UserRole + 115,
        /** Modalias of the device (QString) */
        ModaliasRole = Qt::UserRole + 116,
        /** Last role used by DevicesModel */
        LastRole = Qt::UserRole + 117
    };

    /**
     * Creates a new DevicesModel object.
     *
     * @param manager manager to be used
     * @param parent
     */
    explicit DevicesModel(Manager *manager, QObject *parent = 0);

    /**
      * Reimplemented from QAbstractListModel::roleNames()
      */
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

    /**
      * Reimplemented from QAbstractListModel::rowCount()
      */
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    /**
      * Reimplemented from QAbstractListModel::data()
      */
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

    /**
      * Reimplemented from QAbstractListModel::index()
      */
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    /**
     * Returns a device for specified index.
     *
     * @param index index in model
     * @return device object
     */
    DevicePtr device(const QModelIndex &index) const;

Q_SIGNALS:
    /**
     * Indicates that new device was added.
     */
    void deviceAdded(BluezQt::DevicePtr device);

    /**
     * Indicates that device was removed.
     */
    void deviceRemoved(BluezQt::DevicePtr device);

    /**
     * Indicates that at least one of the device's properties have changed.
     */
    void deviceChanged(BluezQt::DevicePtr device);

private:
    class DevicesModelPrivate *const d;

    friend class DevicesModelPrivate;
};

} // namespace BluezQt

#endif // BLUEZQT_DEVICESMODEL_H
