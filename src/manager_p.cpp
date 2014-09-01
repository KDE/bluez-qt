#include "manager_p.h"
#include "manager.h"
#include "device.h"
#include "adapter.h"
#include "adapter_p.h"
#include "debug_p.h"
#include "utils_p.h"

#include <QDBusReply>
#include <QDBusConnection>
#include <QDBusServiceWatcher>

namespace QBluez
{

ManagerPrivate::ManagerPrivate(Manager *parent)
    : QObject(parent)
    , q(parent)
    , m_dbusObjectManager(0)
    , m_bluezAgentManager(0)
    , m_usableAdapter(0)
    , m_initialized(false)
    , m_bluezRunning(false)
    , m_loaded(false)
    , m_adaptersLoaded(false)
{
    qDBusRegisterMetaType<DBusManagerStruct>();
    qDBusRegisterMetaType<QVariantMapMap>();

    connect(q, &Manager::adapterRemoved, this, &ManagerPrivate::adapterRemoved);
}

void ManagerPrivate::init()
{
    // Keep an eye on bluez service
    QDBusServiceWatcher *serviceWatcher = new QDBusServiceWatcher(Strings::orgBluez(), DBusConnection::orgBluez(),
            QDBusServiceWatcher::WatchForRegistration | QDBusServiceWatcher::WatchForUnregistration, this);

    connect(serviceWatcher, &QDBusServiceWatcher::serviceRegistered, this, &ManagerPrivate::serviceRegistered);
    connect(serviceWatcher, &QDBusServiceWatcher::serviceUnregistered, this, &ManagerPrivate::serviceUnregistered);

    // Update the current state of bluez service
    if (!DBusConnection::orgBluez().isConnected()) {
        Q_EMIT initError(QStringLiteral("DBus system bus is not connected!"));
        return;
    }

    QDBusMessage call = QDBusMessage::createMethodCall(Strings::orgFreedesktopDBus(),
                        QStringLiteral("/"),
                        Strings::orgFreedesktopDBus(),
                        QStringLiteral("NameHasOwner"));

    call << Strings::orgBluez();

    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(DBusConnection::orgBluez().asyncCall(call));
    connect(watcher, &QDBusPendingCallWatcher::finished, this, &ManagerPrivate::nameHasOwnerFinished);
}

void ManagerPrivate::nameHasOwnerFinished(QDBusPendingCallWatcher *watcher)
{
    const QDBusPendingReply<bool> &reply = *watcher;
    watcher->deleteLater();

    if (reply.isError()) {
        Q_EMIT initError(reply.error().message());
        return;
    }

    m_bluezRunning = reply.value();

    if (m_bluezRunning) {
        load();
    } else {
        m_initialized = true;
        Q_EMIT initFinished();
    }
}

void ManagerPrivate::load()
{
    if (!m_bluezRunning || m_loaded) {
        return;
    }

    m_dbusObjectManager = new DBusObjectManager(Strings::orgBluez(), QStringLiteral("/"),
            DBusConnection::orgBluez(), this);

    connect(m_dbusObjectManager, &DBusObjectManager::InterfacesAdded,
            this, &ManagerPrivate::interfacesAdded);
    connect(m_dbusObjectManager, &DBusObjectManager::InterfacesRemoved,
            this, &ManagerPrivate::interfacesRemoved);

    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(m_dbusObjectManager->GetManagedObjects(), this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, &ManagerPrivate::getManagedObjectsFinished);
}

void ManagerPrivate::getManagedObjectsFinished(QDBusPendingCallWatcher *watcher)
{
    const QDBusPendingReply<DBusManagerStruct> &reply = *watcher;
    watcher->deleteLater();

    if (reply.isError()) {
        Q_EMIT initError(reply.error().message());
        return;
    }

    DBusManagerStruct::const_iterator it;
    const DBusManagerStruct &managedObjects = reply.value();

    for (it = managedObjects.constBegin(); it != managedObjects.constEnd(); ++it) {
        const QString &path = it.key().path();
        const QVariantMapMap &interfaces = it.value();

        if (interfaces.contains(Strings::orgBluezAdapter1())) {
            addAdapter(path, interfaces.value(Strings::orgBluezAdapter1()));
        } else if (interfaces.contains(Strings::orgBluezDevice1())) {
            addDevice(path, interfaces.value(Strings::orgBluezDevice1()));
        } else if (interfaces.contains(Strings::orgBluezAgentManager1())) {
            m_bluezAgentManager = new BluezAgentManager(Strings::orgBluez(), path, DBusConnection::orgBluez(), this);
        }
    }

    if (!m_bluezAgentManager) {
        Q_EMIT initError(QStringLiteral("Cannot find org.bluez.AgentManager1 object!"));
        return;
    }

    m_loaded = true;
    m_initialized = true;
    m_usableAdapter = findUsableAdapter();

    Q_EMIT initFinished();
    Q_EMIT q->operationalChanged(true);
}

void ManagerPrivate::clear()
{
    m_loaded = false;

    // Delete all devices first
    Q_FOREACH (Device *device, m_devices.values()) {
        device->adapter()->d->removeDevice(device);
        device->deleteLater();
    }
    m_devices.clear();

    // Delete all adapters
    for (int i = 0; i < m_adapters.count(); ++i) {
        Adapter *adapter = m_adapters.begin().value();
        m_adapters.remove(m_adapters.begin().key());
        Q_EMIT q->adapterRemoved(adapter);
        adapter->deleteLater();
        if (m_adapters.isEmpty()) {
            Q_EMIT q->allAdaptersRemoved();
        }
    }

    // Delete all other objects
    m_usableAdapter = Q_NULLPTR;

    if (m_dbusObjectManager) {
        m_dbusObjectManager->deleteLater();
        m_dbusObjectManager = Q_NULLPTR;
    }

    if (m_bluezAgentManager) {
        m_bluezAgentManager->deleteLater();
        m_bluezAgentManager = Q_NULLPTR;
    }
}

Adapter *ManagerPrivate::findUsableAdapter() const
{
    Q_FOREACH (Adapter *adapter, m_adapters) {
        if (adapter->isPowered()) {
            return adapter;
        }
    }
    return Q_NULLPTR;
}

void ManagerPrivate::serviceRegistered()
{
    qCDebug(QBLUEZ) << "Bluez service registered";
    m_bluezRunning = true;

    load();
}

void ManagerPrivate::serviceUnregistered()
{
    qCDebug(QBLUEZ) << "Bluez service unregistered";
    m_bluezRunning = false;

    clear();
    Q_EMIT q->operationalChanged(false);
}

void ManagerPrivate::interfacesAdded(const QDBusObjectPath &objectPath, const QVariantMapMap &interfaces)
{
    const QString &path = objectPath.path();
    QVariantMapMap::const_iterator it;

    for (it = interfaces.constBegin(); it != interfaces.constEnd(); ++it) {
        if (it.key() == Strings::orgBluezAdapter1()) {
            addAdapter(path, it.value());
        } else if (it.key() == Strings::orgBluezDevice1()) {
            addDevice(path, it.value());
        }
    }
}

void ManagerPrivate::interfacesRemoved(const QDBusObjectPath &objectPath, const QStringList &interfaces)
{
    const QString &path = objectPath.path();

    Q_FOREACH (const QString &interface, interfaces) {
        if (interface == Strings::orgBluezAdapter1()) {
            removeAdapter(path);
        } else if (interface == Strings::orgBluezDevice1()) {
            removeDevice(path);
        }
    }
}

void ManagerPrivate::adapterRemoved(Adapter *adapter)
{
    disconnect(adapter, &Adapter::poweredChanged, this, &ManagerPrivate::adapterPoweredChanged);

    if (adapter == m_usableAdapter) {
        m_usableAdapter = findUsableAdapter();
        Q_EMIT q->usableAdapterChanged(m_usableAdapter);
    }
}

void ManagerPrivate::adapterPoweredChanged(bool powered)
{
    Q_ASSERT(qobject_cast<Adapter*>(sender()));
    Adapter *adapter = static_cast<Adapter*>(sender());

    // Current usable adapter was powered off
    if (m_usableAdapter == adapter && !powered) {
        m_usableAdapter = findUsableAdapter();
        Q_EMIT q->usableAdapterChanged(m_usableAdapter);
    }

    // Adapter was powered on, set it as usable
    if (!m_usableAdapter && powered) {
        m_usableAdapter = adapter;
        Q_EMIT q->usableAdapterChanged(m_usableAdapter);
    }
}

void ManagerPrivate::addAdapter(const QString &adapterPath, const QVariantMap &properties)
{
    Adapter *adapter = new Adapter(adapterPath, properties, this);
    m_adapters.insert(adapterPath, adapter);
    connect(adapter, &Adapter::poweredChanged, this, &ManagerPrivate::adapterPoweredChanged);

    // We will only emit adapterAdded/usableAdapterChanged after loaded (for newly added adapters)
    if (!m_loaded) {
        return;
    }

    Q_EMIT q->adapterAdded(adapter);

    if (!m_usableAdapter && adapter->isPowered()) {
        m_usableAdapter = adapter;
        Q_EMIT q->usableAdapterChanged(m_usableAdapter);
    }
}

void ManagerPrivate::addDevice(const QString &devicePath, const QVariantMap &properties)
{
    Adapter *adapter = m_adapters.value(properties.value(QStringLiteral("Adapter")).value<QDBusObjectPath>().path());
    Q_ASSERT(adapter);
    Device *device = new Device(devicePath, properties, adapter, this);
    adapter->d->addDevice(device);
    m_devices.insert(devicePath, device);
}

void ManagerPrivate::removeAdapter(const QString &adapterPath)
{
    Adapter *adapter = m_adapters.take(adapterPath);
    if (!adapter) {
        return;
    }

    Q_EMIT q->adapterRemoved(adapter);
    adapter->deleteLater();

    if (m_adapters.isEmpty()) {
        Q_EMIT q->allAdaptersRemoved();
    }
}

void ManagerPrivate::removeDevice(const QString &devicePath)
{
    Device *device = m_devices.take(devicePath);
    if (!device) {
        return;
    }

    device->adapter()->d->removeDevice(device);
    device->deleteLater();
}

} // namespace QBluez
