#include "devicesmodel.h"
#include "manager.h"
#include "adapter.h"
#include "device.h"

namespace QBluez
{

class DevicesModelPrivate : public QObject
{
public:
    explicit DevicesModelPrivate(DevicesModel *q);

    void init();

    void adapterAdded(Adapter *adapter);
    void deviceFound(Device *device);
    void deviceRemoved(Device *device);
    void deviceChanged(Device *device);

    DevicesModel *q;
    Manager *m_manager;
    QList<Device*> m_devices;
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

    Q_FOREACH (Adapter *adapter, m_manager->adapters()) {
        adapterAdded(adapter);
    }

    connect(m_manager, &Manager::adapterAdded, this, &DevicesModelPrivate::adapterAdded);
}

void DevicesModelPrivate::adapterAdded(Adapter *adapter)
{
    connect(adapter, &Adapter::deviceFound, this, &DevicesModelPrivate::deviceFound);
    connect(adapter, &Adapter::deviceRemoved, this, &DevicesModelPrivate::deviceRemoved);
    connect(adapter, &Adapter::deviceChanged, this, &DevicesModelPrivate::deviceChanged);
}

void DevicesModelPrivate::deviceFound(Device *device)
{
    q->beginInsertRows(QModelIndex(), m_devices.size(), m_devices.size());
    m_devices.append(device);
    q->endInsertRows();

    Q_EMIT q->deviceFound(device);
}

void DevicesModelPrivate::deviceRemoved(Device *device)
{
    int offset = m_devices.indexOf(device);
    Q_ASSERT(offset >= 0);

    q->beginRemoveRows(QModelIndex(), offset, offset);
    m_devices.removeAt(offset);
    q->endRemoveRows();

    Q_EMIT q->deviceRemoved(device);
}

void DevicesModelPrivate::deviceChanged(Device *device)
{
    int offset = m_devices.indexOf(device);
    Q_ASSERT(offset >= 0);

    QModelIndex idx = q->createIndex(offset, 0, device);
    Q_EMIT q->dataChanged(idx, idx);

    Q_EMIT q->deviceChanged(device);
}

DevicesModel::DevicesModel(Manager *manager, QObject *parent)
    : QAbstractListModel(parent)
    , d(new DevicesModelPrivate(this))
{
    setManager(manager);
}

QHash<int, QByteArray> DevicesModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();

    roles[UbiRole] = QByteArrayLiteral("Ubi");
    roles[AddressRole] = QByteArrayLiteral("Address");
    roles[NameRole] = QByteArrayLiteral("Name");
    roles[FriendlyNameRole] = QByteArrayLiteral("FriendlyName");
    roles[AliasRole] = QByteArrayLiteral("Alias");
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
    roles[AdapterRole] = QByteArrayLiteral("Adapter");

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
    Device *dev = device(index);
    if (!dev) {
        return QVariant();
    }

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
    case AliasRole:
        return dev->alias();
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
    case AdapterRole:
        return QVariant::fromValue(dev->adapter());
    default:
        return QVariant();
    }
}

QModelIndex DevicesModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }
    return createIndex(row, 0, d->m_devices.at(row));
}

Device *DevicesModel::device(const QModelIndex &index) const
{
    return static_cast<Device*>(index.internalPointer());
}

void DevicesModel::setManager(Manager *manager)
{
    if (manager) {
        d->m_manager = manager;
        d->init();
    }
}

} // namespace QBluez
