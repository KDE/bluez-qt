#include "obexmanager_p.h"
#include "obexmanager.h"
#include "debug_p.h"

#include "dbusobjectmanager.h"

#include <QDBusServiceWatcher>

namespace QBluez
{

typedef org::freedesktop::DBus::ObjectManager DBusObjectManager;

ObexManagerPrivate::ObexManagerPrivate(ObexManager *q)
    : QObject(q)
    , q(q)
    , m_obexClient(0)
    , m_obexAgentManager(0)
    , m_dbusObjectManager(0)
    , m_initialized(false)
    , m_obexRunning(false)
    , m_loaded(false)
{
    qDBusRegisterMetaType<DBusManagerStruct>();
    qDBusRegisterMetaType<QVariantMapMap>();

    m_timer = new QTimer(this);
    m_timer->setSingleShot(true);
    m_timer->setInterval(500);
    connect(m_timer, &QTimer::timeout, this, &ObexManagerPrivate::load);
}

ObexManagerPrivate::~ObexManagerPrivate()
{
    clear();
}

void ObexManagerPrivate::init()
{
    // Keep an eye on bluez.obex service
    QDBusServiceWatcher *serviceWatcher = new QDBusServiceWatcher(QStringLiteral("org.bluez.obex"), QDBusConnection::sessionBus(),
            QDBusServiceWatcher::WatchForRegistration | QDBusServiceWatcher::WatchForUnregistration, this);

    connect(serviceWatcher, &QDBusServiceWatcher::serviceRegistered, this, &ObexManagerPrivate::serviceRegistered);
    connect(serviceWatcher, &QDBusServiceWatcher::serviceUnregistered, this, &ObexManagerPrivate::serviceUnregistered);

    // Update the current state of bluez.obex service
    if (!QDBusConnection::sessionBus().isConnected()) {
        Q_EMIT initError(QStringLiteral("DBus session bus is not connected!"));
        return;
    }

    QDBusMessage call = QDBusMessage::createMethodCall(QStringLiteral("org.freedesktop.DBus"),
                        QStringLiteral("/"),
                        QStringLiteral("org.freedesktop.DBus"),
                        QStringLiteral("NameHasOwner"));
    QList<QVariant> args;
    args.append(QStringLiteral("org.bluez.obex"));
    call.setArguments(args);

    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(QDBusConnection::sessionBus().asyncCall(call));
    connect(watcher, &QDBusPendingCallWatcher::finished, this, &ObexManagerPrivate::nameHasOwnerFinished);
}

void ObexManagerPrivate::nameHasOwnerFinished(QDBusPendingCallWatcher *watcher)
{
    const QDBusPendingReply<bool> &reply = *watcher;
    watcher->deleteLater();

    if (reply.isError()) {
        Q_EMIT initError(reply.error().message());
        return;
    }

    m_initialized = true;
    m_obexRunning = reply.value();

    if (m_obexRunning) {
        load();
    } else {
        Q_EMIT initFinished();
    }
}

void ObexManagerPrivate::load()
{
    if (!m_obexRunning || m_loaded) {
        return;
    }

    m_dbusObjectManager = new DBusObjectManager(QStringLiteral("org.bluez.obex"), QStringLiteral("/"),
            QDBusConnection::sessionBus(), this);

    connect(m_dbusObjectManager, &DBusObjectManager::InterfacesRemoved,
            this, &ObexManagerPrivate::interfacesRemoved);

    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(m_dbusObjectManager->GetManagedObjects(), this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, &ObexManagerPrivate::getManagedObjectsFinished);
}

void ObexManagerPrivate::getManagedObjectsFinished(QDBusPendingCallWatcher *watcher)
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

        if (interfaces.contains(QStringLiteral("org.bluez.obex.Client1"))) {
            m_obexClient = new ObexClient(QStringLiteral("org.bluez.obex"), path, QDBusConnection::sessionBus(), this);
        }
        if (interfaces.contains(QStringLiteral("org.bluez.obex.AgentManager1"))) {
            m_obexAgentManager = new ObexAgentManager(QStringLiteral("org.bluez.obex"), path, QDBusConnection::sessionBus(), this);
        }
    }

    Q_ASSERT(m_obexClient);
    Q_ASSERT(m_obexAgentManager);

    if (!m_obexClient) {
        Q_EMIT initError(QStringLiteral("Cannot find org.bluez.obex.Client1 object!"));
        return;
    }

    if (!m_obexAgentManager) {
        Q_EMIT initError(QStringLiteral("Cannot find org.bluez.obex.AgentManager1 object!"));
        return;
    }

    m_loaded = true;
    Q_EMIT initFinished();
    Q_EMIT q->operationalChanged(true);
}

void ObexManagerPrivate::clear()
{
    m_loaded = false;

    if (m_obexClient) {
        m_obexClient->deleteLater();
        m_obexClient = Q_NULLPTR;
    }

    if (m_obexAgentManager) {
        m_obexAgentManager->deleteLater();
        m_obexAgentManager = Q_NULLPTR;
    }

    if (m_dbusObjectManager) {
        m_dbusObjectManager->deleteLater();
        m_dbusObjectManager = Q_NULLPTR;
    }
}

void ObexManagerPrivate::serviceRegistered()
{
    qCDebug(QBLUEZ) << "Obex service registered";
    m_obexRunning = true;

    // Client1 and AgentManager1 objects are not ready by the time org.bluez.obex is registered
    // nor will the ObjectManager emits interfacesAdded for adding them...
    // So we delay the call to load() by 0.5s
    m_timer->start();
}

void ObexManagerPrivate::serviceUnregistered()
{
    qCDebug(QBLUEZ) << "Obex service unregistered";
    m_obexRunning = false;

    clear();
    Q_EMIT q->operationalChanged(false);
}

void ObexManagerPrivate::interfacesRemoved(const QDBusObjectPath &objectPath, const QStringList &interfaces)
{
    if (interfaces.contains(QStringLiteral("org.bluez.obex.Session1"))) {
        Q_EMIT q->sessionRemoved(objectPath);
    }
}

} // namespace QBluez
