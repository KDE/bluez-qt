#include "manager_p.h"
#include "deviceinfo.h"
#include "adapterinfo.h"

#include <QDebug>
#include <QtDBus/QDBusReply>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusConnectionInterface>
#include <QtDBus/QDBusServiceWatcher>

using namespace QBluez;

ManagerPrivate::ManagerPrivate(Manager *parent)
    : QObject(parent)
    , q(parent)
    , m_dbusObjectManager(0)
    , m_bluezAgentManager(0)
    , m_bluezRunning(false)
    , m_initialized(false)
{
    qDBusRegisterMetaType<DBusManagerStruct>();
    qDBusRegisterMetaType<QVariantMapMap>();

    // Keep an eye on bluez service
    QDBusServiceWatcher *serviceWatcher = new QDBusServiceWatcher(QStringLiteral("org.bluez"), QDBusConnection::systemBus(),
            QDBusServiceWatcher::WatchForRegistration | QDBusServiceWatcher::WatchForUnregistration, this);

    connect(serviceWatcher, &QDBusServiceWatcher::serviceRegistered, [ = ]() {
        m_bluezRunning = true;
        initialize();
    });

    connect(serviceWatcher, &QDBusServiceWatcher::serviceUnregistered, [ = ]() {
        m_bluezRunning = false;
        clear();
    });

    // Update the current state of bluez service
    if (QDBusConnection::systemBus().isConnected()) {
        QDBusReply<bool> reply = QDBusConnection::systemBus().interface()->isServiceRegistered(QStringLiteral("org.bluez"));
        m_bluezRunning = reply.isValid() && reply.value();
    }
}

void ManagerPrivate::initialize()
{
    if (!m_bluezRunning || m_initialized) {
        return;
    }

    m_dbusObjectManager = new DBusObjectManager(QStringLiteral("org.bluez"), QStringLiteral("/"),
            QDBusConnection::systemBus(), this);

    connect(m_dbusObjectManager, &DBusObjectManager::InterfacesAdded,
            this, &ManagerPrivate::interfacesAdded);
    connect(m_dbusObjectManager, &DBusObjectManager::InterfacesRemoved,
            this, &ManagerPrivate::interfacesRemoved);

    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(m_dbusObjectManager->GetManagedObjects(), this);

    connect(watcher, &QDBusPendingCallWatcher::finished, [ = ]() {
        QDBusPendingReply<DBusManagerStruct> reply = *watcher;

        if (reply.isError()) {
            qWarning() << "Error:" << reply.error().message();
        } else {
            DBusManagerStruct::const_iterator it;
            const DBusManagerStruct &managedObjects = reply.value();

            for (it = managedObjects.constBegin(); it != managedObjects.constEnd(); ++it) {
                const QString &path = it.key().path();
                const QVariantMapMap &interfaces = it.value();

                if (interfaces.contains(QStringLiteral("org.bluez.Adapter1"))) {
                    m_adapters.insert(path, new AdapterInfo(path));
                } else if (interfaces.contains(QStringLiteral("org.bluez.Device1"))) {
                    const QString &adapterPath = it.value().value(QStringLiteral("org.bluez.Device1")).value(QStringLiteral("Adapter")).value<QDBusObjectPath>().path();
                    m_adapters[adapterPath]->addDevice(new DeviceInfo(path));
                } else if (interfaces.contains(QStringLiteral("org.bluez.AgentManager1"))) {
                    m_bluezAgentManager = new BluezAgentManager(QStringLiteral("org.bluez"), path, QDBusConnection::systemBus(), this);
                }
            }

            qDebug("ManagerPrivate: Initialized");

            m_initialized = true;
            emit q->operationalChanged(true);
        }

        delete watcher;
    });
}

void ManagerPrivate::clear()
{
    qDebug("ManagerPrivate: Clear");

    m_initialized = false;
    m_adapters.clear();
    emit q->operationalChanged(false);
}

void ManagerPrivate::interfacesAdded(const QDBusObjectPath &objectPath, const QVariantMapMap &interfaces)
{
    const QString &object = objectPath.path();
    QVariantMapMap::const_iterator it;

    for (it = interfaces.constBegin(); it != interfaces.constEnd(); ++it) {
        if (it.key() == QLatin1String("org.bluez.Adapter1")) {
            AdapterInfo *adapter = new AdapterInfo(object, this);
            m_adapters.insert(object, adapter);
            emit q->adapterAdded(adapter);
        } else if (it.key() == QLatin1String("org.bluez.Device1")) {
            const QString &adapterPath = it.value().value(QStringLiteral("Adapter")).value<QDBusObjectPath>().path();
            m_adapters[adapterPath]->addDevice(new DeviceInfo(object));
        }
    }
}

void ManagerPrivate::interfacesRemoved(const QDBusObjectPath &objectPath, const QStringList &interfaces)
{
    const QString &object = objectPath.path();

    Q_FOREACH (const QString &interface, interfaces) {
        if (interface == QLatin1String("org.bluez.Adapter1")) {
            AdapterInfo *adapter = m_adapters.take(object);
            Q_ASSERT(adapter);

            emit q->adapterRemoved(adapter);
            delete adapter;
        } else if (interface == QLatin1String("org.bluez.Device1")) {
            Q_FOREACH (AdapterInfo *adapter, m_adapters.values()) {
                Q_FOREACH (DeviceInfo *device, adapter->devices()) {
                    if (device->path() == object) {
                        adapter->removeDevice(device);
                        delete device;
                        break;
                    }
                }
            }
        }
    }
}
