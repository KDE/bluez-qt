#include "adapter_p.h"
#include "adapter.h"

using namespace QBluez;

AdapterPrivate::AdapterPrivate(const QString &path, Adapter *parent)
    : QObject(parent)
    , q(parent)
    , m_loaded(false)
    , m_path(path)
    , m_adapterClass(0)
    , m_powered(0)
    , m_discoverable(false)
    , m_discoverableTimeout(0)
    , m_pairable(false)
    , m_pairableTimeout(0)
{
    m_bluezAdapter = new BluezAdapter(QStringLiteral("org.bluez"), m_path,
                                      QDBusConnection::systemBus(), this);
    m_dbusProperties = new DBusProperties(QStringLiteral("org.bluez"), m_path,
                                          QDBusConnection::systemBus(), this);

    // QueuedConnection is important here to be able to perform actions, that depend on
    // a previously set property, directly from slot connected to propertyChanged signal.
    // Eg. Powering on adapter and then starting discovery.
    //  * with DirectConnection the StartDiscovery would fail because the adapter is still
    //    powered off when the PropertiesChanged signal is emitted ...
    connect(m_dbusProperties, &DBusProperties::PropertiesChanged,
            this, &AdapterPrivate::propertiesChanged, Qt::QueuedConnection);
}

void AdapterPrivate::addDevice(Device *device)
{
    m_devices.append(device);
    Q_EMIT q->deviceFound(device);
}

void AdapterPrivate::removeDevice(Device *device)
{
    m_devices.removeOne(device);
    Q_EMIT q->deviceRemoved(device);
}

QDBusPendingReply<> AdapterPrivate::setDBusProperty(const QString &name, const QVariant &value)
{
    return m_dbusProperties->Set(QStringLiteral("org.bluez.Adapter1"), name, QDBusVariant(value));
}

void AdapterPrivate::propertiesChanged(const QString &interface, const QVariantMap &changed, const QStringList &invalidated)
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
        } else if (property == QLatin1String("Alias")) {
            m_alias = value.toString();
            Q_EMIT q->aliasChanged(m_alias);
        } else if (property == QLatin1String("Class")) {
            m_adapterClass = value.toUInt();
            Q_EMIT q->adapterClassChanged(m_adapterClass);
        } else if (property == QLatin1String("Powered")) {
            m_powered = value.toBool();
            Q_EMIT q->poweredChanged(m_powered);
        } else if (property == QLatin1String("Discoverable")) {
            m_discoverable = value.toBool();
            Q_EMIT q->discoverableChanged(m_discoverable);
        } else if (property == QLatin1String("DiscoverableTimeout")) {
            m_discoverableTimeout = value.toUInt();
            Q_EMIT q->discoverableTimeoutChanged(m_discoverableTimeout);
        } else if (property == QLatin1String("Pairable")) {
            m_pairable = value.toBool();
            Q_EMIT q->pairableChanged(m_pairable);
        } else if (property == QLatin1String("PairableTimeout")) {
            m_pairableTimeout = value.toUInt();
            Q_EMIT q->pairableTimeoutChanged(m_pairableTimeout);
        } else if (property == QLatin1String("Discovering")) {
            m_discovering = value.toBool();
            Q_EMIT q->discoveringChanged(m_discovering);
        } else if (property == QLatin1String("UUIDs")) {
            m_UUIDs = value.toStringList();
            Q_EMIT q->UUIDsChanged(m_UUIDs);
        } else if (property == QLatin1String("Modalias")) {
            m_modalias = value.toString();
            Q_EMIT q->modaliasChanged(m_modalias);
        }
    }
}
