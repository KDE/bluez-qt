#include "device_p.h"
#include "device.h"
#include "adapter.h"

using namespace QBluez;

DevicePrivate::DevicePrivate(const QString &path, Adapter *adapter, Device *parent)
    : QObject(parent)
    , q(parent)
    , m_dbusProperties(0)
    , m_loaded(false)
    , m_path(path)
    , m_deviceClass(0)
    , m_appearance(0)
    , m_paired(false)
    , m_trusted(false)
    , m_blocked(false)
    , m_legacyPairing(false)
    , m_rssi(-1)
    , m_connected(false)
    , m_adapter(adapter)
{
    m_bluezDevice = new BluezDevice(QStringLiteral("org.bluez"), m_path,
                                    QDBusConnection::systemBus(), this);
}

void DevicePrivate::initProperties()
{
    m_dbusProperties = new DBusProperties(QStringLiteral("org.bluez"), m_path,
                                          QDBusConnection::systemBus(), this);

    // QueuedConnection is important here - see AdapterPrivate::initProperties
    connect(m_dbusProperties, &DBusProperties::PropertiesChanged,
            this, &DevicePrivate::propertiesChanged, Qt::QueuedConnection);
}

QDBusPendingReply<> DevicePrivate::setDBusProperty(const QString &name, const QVariant &value)
{
    return m_dbusProperties->Set(QStringLiteral("org.bluez.Device1"), name, QDBusVariant(value));
}

void DevicePrivate::propertiesChanged(const QString &interface, const QVariantMap &changed, const QStringList &invalidated)
{
    Q_UNUSED(interface)
    Q_UNUSED(invalidated)

    QVariantMap::const_iterator i;
    for (i = changed.constBegin(); i != changed.constEnd(); ++i) {
        const QVariant &value = i.value();
        const QString &property = i.key();

        if (property == QLatin1String("Name")) {
            m_name = value.toString();
            Q_EMIT q->nameChanged(m_name);
            Q_EMIT q->friendlyNameChanged(q->friendlyName());
        } else if (property == QLatin1String("Alias")) {
            m_alias = value.toString();
            Q_EMIT q->aliasChanged(m_alias);
            Q_EMIT q->friendlyNameChanged(q->friendlyName());
        } else if (property == QLatin1String("Class")) {
            m_deviceClass = value.toUInt();
            Q_EMIT q->deviceClassChanged(m_deviceClass);
        } else if (property == QLatin1String("Appearance")) {
            m_appearance = value.toUInt();
            Q_EMIT q->appearanceChanged(m_appearance);
        } else if (property == QLatin1String("Icon")) {
            m_icon = value.toString();
            Q_EMIT q->iconChanged(m_icon);
        } else if (property == QLatin1String("Paired")) {
            m_paired = value.toBool();
            Q_EMIT q->pairedChanged(m_paired);
        } else if (property == QLatin1String("Trusted")) {
            m_trusted = value.toBool();
            Q_EMIT q->trustedChanged(m_trusted);
        } else if (property == QLatin1String("Blocked")) {
            m_blocked = value.toBool();
            Q_EMIT q->blockedChanged(m_blocked);
        } else if (property == QLatin1String("LegacyPairing")) {
            m_legacyPairing = value.toBool();
            Q_EMIT q->legacyPairingChanged(m_legacyPairing);
        } else if (property == QLatin1String("RSSI")) {
            m_rssi = value.toInt();
            Q_EMIT q->rssiChanged(m_rssi);
        } else if (property == QLatin1String("Connected")) {
            m_connected = value.toBool();
            Q_EMIT q->connectedChanged(m_connected);
        } else if (property == QLatin1String("UUIDs")) {
            m_uuids = value.toStringList();
            Q_EMIT q->uuidsChanged(m_uuids);
        } else if (property == QLatin1String("Modalias")) {
            m_modalias = value.toString();
            Q_EMIT q->modaliasChanged(m_modalias);
        }
    }

    Q_EMIT q->adapter()->deviceChanged(q);
}
