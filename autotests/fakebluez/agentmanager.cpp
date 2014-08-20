#include "agentmanager.h"

#include <QDebug>

AgentManager::AgentManager(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    setName(QStringLiteral("org.bluez.AgentManager1"));
    setPath(QDBusObjectPath(QStringLiteral("/org/bluez")));
}

void AgentManager::RegisterAgent(const QDBusObjectPath &path, const QString &capability)
{
    qDebug() << "RegisterAgent" << path.path() << capability;
}

void AgentManager::UnregisterAgent(const QDBusObjectPath &path)
{
    qDebug() << "UnregisterAgent" << path.path();
}

void AgentManager::RequestDefaultAgent(const QDBusObjectPath &path)
{
    qDebug() << "RequestDefaultAgent" << path.path();
}
