#ifndef QBLUEZ_DEVICESMODEL_H
#define QBLUEZ_DEVICESMODEL_H

#include <QAbstractListModel>

#include "qbluez_export.h"

namespace QBluez
{

class Manager;
class Device;

class QBLUEZ_EXPORT DevicesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum DeviceRoles {
        UbiRole = Qt::UserRole + 100,
        AddressRole = Qt::UserRole + 101,
        NameRole = Qt::UserRole + 102,
        FriendlyNameRole = Qt::UserRole + 103,
        AliasRole = Qt::UserRole + 104,
        DeviceClassRole = Qt::UserRole + 105,
        DeviceTypeRole = Qt::UserRole + 106,
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
        AdapterRole = Qt::UserRole + 117,
        LastRole = Qt::UserRole + 118
    };

    explicit DevicesModel(Manager *manager, QObject *parent = 0);

    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    Device *device(const QModelIndex &index) const;

Q_SIGNALS:
    void deviceFound(QBluez::Device *device);
    void deviceRemoved(QBluez::Device *device);
    void deviceChanged(QBluez::Device *device);

private:
    class DevicesModelPrivate *const d;

    friend class DevicesModelPrivate;
};

} // namespace QBluez

#endif // QBLUEZ_DEVICESMODEL_H
