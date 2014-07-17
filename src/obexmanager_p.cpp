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
    , m_initialized(false)
    , m_obexRunning(false)
    , m_loaded(false)
{
    qDBusRegisterMetaType<DBusManagerStruct>();
    qDBusRegisterMetaType<QVariantMapMap>();
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

    connect(serviceWatcher, &QDBusServiceWatcher::serviceRegistered, [ this ]() {
        qCDebug(QBLUEZ) << "Manager: Obex service registered";
        m_obexRunning = true;
        load();
        Q_EMIT q->operationalChanged(m_obexRunning);
    });

    connect(serviceWatcher, &QDBusServiceWatcher::serviceUnregistered, [ this ]() {
        qCDebug(QBLUEZ) << "Manager: Obex service unregistered";
        m_obexRunning = false;
        clear();
        Q_EMIT q->operationalChanged(m_obexRunning);
    });

    // Update the current state of bluez.obex service
    if (QDBusConnection::sessionBus().isConnected()) {
        QDBusMessage call = QDBusMessage::createMethodCall(QStringLiteral("org.freedesktop.DBus"),
                            QStringLiteral("/"),
                            QStringLiteral("org.freedesktop.DBus"),
                            QStringLiteral("NameHasOwner"));
        QList<QVariant> args;
        args.append(QStringLiteral("org.bluez.obex"));
        call.setArguments(args);

        QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(QDBusConnection::sessionBus().asyncCall(call));

        connect(watcher, &QDBusPendingCallWatcher::finished, [ this, watcher ]() {
            const QDBusPendingReply<bool> &reply = *watcher;
            watcher->deleteLater();

            if (reply.isError()) {
                Q_EMIT initError(reply.error().message());
            } else {
                m_initialized = true;
                m_obexRunning = reply.isValid() && reply.value();
                if (m_obexRunning) {
                    load();
                } else {
                    Q_EMIT initFinished();
                }
            }
        });
    } else {
        Q_EMIT initError(QStringLiteral("DBus session bus is not connected!"));
    }
}

void ObexManagerPrivate::load()
{
    if (!m_obexRunning || m_loaded) {
        return;
    }

    DBusObjectManager objectManager(QStringLiteral("org.bluez.obex"), QStringLiteral("/"),
            QDBusConnection::sessionBus());

    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(objectManager.GetManagedObjects(), this);

    connect(watcher, &QDBusPendingCallWatcher::finished, [ this, watcher ]() {
        const QDBusPendingReply<DBusManagerStruct> &reply = *watcher;
        watcher->deleteLater();

        if (reply.isError()) {
            Q_EMIT initError(reply.error().message());
        } else {
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
        }
    });
}

void ObexManagerPrivate::clear()
{
    m_loaded = false;

    m_obexClient->deleteLater();
    m_obexClient = Q_NULLPTR;

    m_obexAgentManager->deleteLater();
    m_obexAgentManager = Q_NULLPTR;
}

} // namespace QBluez
