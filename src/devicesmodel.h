#ifndef QBLUEZ_DEVICESMODEL_H
#define QBLUEZ_DEVICESMODEL_H

#include <QAbstractListModel>

#include "qbluez_export.h"

namespace QBluez
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
class QBLUEZ_EXPORT DevicesModel : public QAbstractListModel
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
        /** Alias of the device (QString) */
        AliasRole = Qt::UserRole + 104,
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
        /** Adapter that discovered the device (Adapter) */
        AdapterRole = Qt::UserRole + 117,
        /** Last role useb by DevicesModel */
        LastRole = Qt::UserRole + 118
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
    Device *device(const QModelIndex &index) const;

Q_SIGNALS:
    /**
     * Indicates that new device was found.
     */
    void deviceFound(QBluez::Device *device);

    /**
     * Indicates that device was removed.
     */
    void deviceRemoved(QBluez::Device *device);

    /**
     * Indicates that at least one of the device's property have changed.
     */
    void deviceChanged(QBluez::Device *device);

private:
    class DevicesModelPrivate *const d;

    friend class DevicesModelPrivate;
};

} // namespace QBluez

#endif // QBLUEZ_DEVICESMODEL_H
