#include "device_p.h"
#include "device.h"
#include "adapter.h"
#include "utils_p.h"

namespace QBluez
{

DevicePrivate::DevicePrivate(const QString &path, Adapter *adapter, Device *parent)
    : QObject(parent)
    , q(parent)
    , m_dbusProperties(0)
    , m_loaded(false)
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
    m_bluezDevice = new BluezDevice(QStringLiteral("org.bluez"), path,
                                    QDBusConnection::systemBus(), this);
}

void DevicePrivate::load()
{
    m_dbusProperties = new DBusProperties(QStringLiteral("org.bluez"), m_bluezDevice->path(),
                                          QDBusConnection::systemBus(), this);

    // QueuedConnection is important here - see AdapterPrivate::initProperties
    connect(m_dbusProperties, &DBusProperties::PropertiesChanged,
            this, &DevicePrivate::propertiesChanged, Qt::QueuedConnection);

    const QDBusPendingReply<QVariantMap> &call = m_dbusProperties->GetAll(QStringLiteral("org.bluez.Device1"));
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, &DevicePrivate::getPropertiesFinished);
}

void DevicePrivate::getPropertiesFinished(QDBusPendingCallWatcher *watcher)
{
    const QDBusPendingReply<QVariantMap> &reply = *watcher;
    watcher->deleteLater();

    if (reply.isError()) {
        Q_EMIT loadError(reply.error().message());
        return;
    }

    const QVariantMap &properties = reply.value();

    m_address = properties.value(QStringLiteral("Address")).toString();
    m_name = properties.value(QStringLiteral("Name")).toString();
    m_alias = properties.value(QStringLiteral("Alias")).toString();
    m_deviceClass = properties.value(QStringLiteral("Class")).toUInt();
    m_appearance = properties.value(QStringLiteral("Appearance")).toUInt();
    m_icon = properties.value(QStringLiteral("Icon")).toString();
    m_paired = properties.value(QStringLiteral("Paired")).toBool();
    m_trusted = properties.value(QStringLiteral("Trusted")).toBool();
    m_blocked = properties.value(QStringLiteral("Blocked")).toBool();
    m_legacyPairing = properties.value(QStringLiteral("LegacyPairing")).toBool();
    m_rssi = properties.value(QStringLiteral("RSSI")).toInt();
    m_connected = properties.value(QStringLiteral("Connected")).toBool();
    m_uuids = stringListToUpper(properties.value(QStringLiteral("UUIDs")).toStringList());
    m_modalias = properties.value(QStringLiteral("Modalias")).toString();

    m_loaded = true;

    Q_EMIT loaded(this);
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
            if (m_deviceClass != value.toUInt()) {
                m_deviceClass = value.toUInt();
                Q_EMIT q->deviceClassChanged(m_deviceClass);
                Q_EMIT q->deviceTypeChanged(q->deviceType());
            }
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
        } else if (property == QLatin1String("Modalias")) {
            PROPERTY_CHANGED(m_modalias, toString, modaliasChanged);
        } else if (property == QLatin1String("UUIDs")) {
            const QStringList &changedUuids = stringListToUpper(value.toStringList());
            if (m_uuids != changedUuids) {
                m_uuids = changedUuids;
                Q_EMIT q->uuidsChanged(m_uuids);
            }
        }
    }

    Q_EMIT q->deviceChanged(q);
    Q_EMIT m_adapter->deviceChanged(q);
}

#undef PROPERTY_CHANGED

} // namespace QBluez
