#include "device_p.h"
#include "device.h"
#include "adapter.h"

namespace QBluez
{

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

// Make sure not to emit propertyChanged signal when the property already contains changed value
#define PROPERTY_CHANGED(var, type_cast, signal) \
    if (var != value.type_cast()) { \
        var = value.type_cast(); \
        Q_EMIT q->signal(var); \
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
            if (m_name != value.toString()) {
                m_name = value.toString();
                Q_EMIT q->nameChanged(m_name);
                Q_EMIT q->friendlyNameChanged(q->friendlyName());
            }
        } else if (property == QLatin1String("Alias")) {
            if (m_alias != value.toString()) {
                m_alias = value.toString();
                Q_EMIT q->aliasChanged(m_alias);
                Q_EMIT q->friendlyNameChanged(q->friendlyName());
            }
        } else if (property == QLatin1String("Class")) {
            PROPERTY_CHANGED(m_deviceClass, toUInt, deviceClassChanged);
        } else if (property == QLatin1String("Appearance")) {
            PROPERTY_CHANGED(m_appearance, toUInt, appearanceChanged);
        } else if (property == QLatin1String("Icon")) {
            PROPERTY_CHANGED(m_icon, toString, iconChanged);
        } else if (property == QLatin1String("Paired")) {
            PROPERTY_CHANGED(m_paired, toBool, pairedChanged);
        } else if (property == QLatin1String("Trusted")) {
            PROPERTY_CHANGED(m_trusted, toBool, trustedChanged);
        } else if (property == QLatin1String("Blocked")) {
            PROPERTY_CHANGED(m_blocked, toBool, blockedChanged);
        } else if (property == QLatin1String("LegacyPairing")) {
            PROPERTY_CHANGED(m_legacyPairing, toBool, legacyPairingChanged);
        } else if (property == QLatin1String("RSSI")) {
            PROPERTY_CHANGED(m_rssi, toInt, rssiChanged);
        } else if (property == QLatin1String("Connected")) {
            PROPERTY_CHANGED(m_connected, toBool, connectedChanged);
        } else if (property == QLatin1String("UUIDs")) {
            PROPERTY_CHANGED(m_uuids, toStringList, uuidsChanged);
        } else if (property == QLatin1String("Modalias")) {
            PROPERTY_CHANGED(m_modalias, toString, modaliasChanged);
        }
    }

    Q_EMIT m_adapter->deviceChanged(q);
}

#undef PROPERTY_CHANGED

} // namespace QBluez
