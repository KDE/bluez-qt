#include "manager_p.h"

#include <QtDBus/QDBusReply>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusConnectionInterface>
#include <QtDBus/QDBusServiceWatcher>

using namespace QBluez;

ManagerPrivate::ManagerPrivate(QObject *parent)
    : QObject(parent)
    , m_bluezRunning(false)
{
    // Keep an eye on bluez service
    QDBusServiceWatcher *serviceWatcher = new QDBusServiceWatcher(QStringLiteral("org.bluez"), QDBusConnection::systemBus(),
                                                                  QDBusServiceWatcher::WatchForRegistration |
                                                                  QDBusServiceWatcher::WatchForUnregistration, this);

    connect(serviceWatcher, &QDBusServiceWatcher::serviceRegistered, [=]() {
        m_bluezRunning = true;
        qDebug("Service registered");
    });

    connect(serviceWatcher, &QDBusServiceWatcher::serviceUnregistered, [=]() {
        m_bluezRunning = false;
        qDebug("Service unregistered");
    });

    // Update the current state of bluez service
    if (QDBusConnection::systemBus().isConnected()) {
        QDBusReply<bool> reply = QDBusConnection::systemBus().interface()->isServiceRegistered(QStringLiteral("org.bluez"));
        m_bluezRunning = reply.isValid() && reply.value();
    }
}

void ManagerPrivate::initialize()
{
}
