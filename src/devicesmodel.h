/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * SPDX-FileCopyrightText: 2014-2015 David Rosca <nowrep@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef BLUEZQT_DEVICESMODEL_H
#define BLUEZQT_DEVICESMODEL_H

#include <QAbstractListModel>

#include "bluezqt_export.h"
#include "types.h"

#include <memory>

namespace BluezQt
{
class Manager;
class Device;

/*!
 * \inmodule BluezQt
 * \class BluezQt::DevicesModel
 * \inheaderfile BluezQt/DevicesModel
 *
 * \brief Model of all devices.
 *
 * This class represents a model of all devices.
 *
 * Example use in QML code:
 * \code
 * import org.kde.bluezqt 1.0 as BluezQt
 *
 * ListView {
 *     model: BluezQt.DevicesModel { }
 *     delegate: Text {
 *         text: "%1 (%2)".arg(Name).arg(Address)
 *     }
 * }
 * \endcode
 */
class BLUEZQT_EXPORT DevicesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    /*!
     * Device data roles.
     *
     * \value UbiRole
     *        UBI of the device (QString).
     * \value AddressRole
     *        Address of the device (QString).
     * \value NameRole
     *        Name of the device (QString).
     * \value FriendlyNameRole
     *        Friendly name of the device (QString).
     * \value RemoteNameRole
     *        Remote name of the device (QString).
     * \value ClassRole
     *        Class of the device (quint32).
     * \value TypeRole
     *        Type of the device (Device::Type).
     * \value AppearanceRole
     *        Appearance of the device (quint16).
     * \value IconRole
     *        Icon name of the device (QString).
     * \value PairedRole
     *        Indicates whether the device is paired (bool).
     * \value TrustedRole
     *        Indicates whether the device is trusted (bool).
     * \value BlockedRole
     *        Indicates whether the device is blocked (bool).
     * \value LegacyPairingRole
     *        Indicates whether the device has legacy pairing (bool).
     * \value RssiRole
     *        Received Signal Strength Indicator of the device (qint16).
     * \value ConnectedRole
     *        Indicates whether the device is connected (bool).
     * \value UuidsRole
     *        UUIDs of supported services by the device (QStringList).
     * \value ModaliasRole
     *        Modalias of the device (QString).
     * \value AdapterNameRole
     *        Name of the associated adapter (QString).
     * \value AdapterAddressRole
     *        Address of the associated adapter (QString).
     * \value AdapterPoweredRole
     *        Indicates whether the associated adapter is powered (bool).
     * \value AdapterDiscoverableRole
     *        Indicates whether the associated adapter is discoverable (bool).
     * \value AdapterPairableRole
     *        Indicates whether the associated adapter is pairable (bool).
     * \value AdapterDiscoveringRole
     *        Indicates whether the associated adapter is discovering (bool).
     * \value AdapterUuidsRole
     *        UUIDs of supported services by the associated adapter (QStringList).
     * \value LastRole
     *        Last role used by DevicesModel.
     */
    enum DeviceRoles {
        UbiRole = Qt::UserRole + 100,
        AddressRole = Qt::UserRole + 101,
        NameRole = Qt::UserRole + 102,
        FriendlyNameRole = Qt::UserRole + 103,
        RemoteNameRole = Qt::UserRole + 104,
        ClassRole = Qt::UserRole + 105,
        TypeRole = Qt::UserRole + 106,
        AppearanceRole = Qt::UserRole + 107,
        IconRole = Qt::UserRole + 108,
        PairedRole = Qt::UserRole + 109,
        TrustedRole = Qt::UserRole + 110,
        BlockedRole = Qt::UserRole + 111,
        LegacyPairingRole = Qt::UserRole + 112,
        RssiRole = Qt::UserRole + 113,
        ConnectedRole = Qt::UserRole + 114,
        UuidsRole = Qt::UserRole + 115,
        ModaliasRole = Qt::UserRole + 116,
        AdapterNameRole = Qt::UserRole + 117,
        AdapterAddressRole = Qt::UserRole + 118,
        AdapterPoweredRole = Qt::UserRole + 119,
        AdapterDiscoverableRole = Qt::UserRole + 120,
        AdapterPairableRole = Qt::UserRole + 121,
        AdapterDiscoveringRole = Qt::UserRole + 122,
        AdapterUuidsRole = Qt::UserRole + 123,
        LastRole = Qt::UserRole + 124,
    };

    /*!
     * Creates a new DevicesModel object using the given \a manager as a child of \a parent.
     */
    explicit DevicesModel(Manager *manager, QObject *parent = nullptr);

    ~DevicesModel() override;

    QHash<int, QByteArray> roleNames() const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

    /*!
     * Returns a device object for the specified \a index in the model.
     */
    DevicePtr device(const QModelIndex &index) const;

private:
    std::unique_ptr<class DevicesModelPrivate> const d;

    friend class DevicesModelPrivate;
};

} // namespace BluezQt

#endif // BLUEZQT_DEVICESMODEL_H
