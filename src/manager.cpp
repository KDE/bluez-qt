#include "manager.h"
#include "manager_p.h"
#include "adapter.h"
#include "loadadaptersjob.h"

using namespace QBluez;

static Manager *instance = 0;

Manager::Manager()
    : QObject()
    , d(new ManagerPrivate(this))
{
}

Manager::~Manager()
{
    delete d;
}

// static
Manager *Manager::self()
{
    if (!instance) {
        instance = new Manager();
    }
    return instance;
}

// static
void Manager::release()
{
    delete instance;
    instance = 0;
}

QList<Adapter *> Manager::adapters() const
{
    return d->m_adapters.values();
}

QList<Device *> Manager::devices() const
{
    QList<Device *> list;

    Q_FOREACH (Adapter *adapter, d->m_adapters.values()) {
        list.append(adapter->devices());
    }

    return list;
}

LoadAdaptersJob *Manager::loadAdapters()
{
    return new LoadAdaptersJob(d, this);
}

Adapter *Manager::usableAdapter()
{
    return d->usableAdapter();
}

bool Manager::isOperational() const
{
    return d->m_bluezRunning && d->m_initialized;
}

void Manager::registerAgent(const QString &agentPath, RegisterCapability registerCapability)
{
    QString capability;

    switch (registerCapability) {
    case DisplayOnly:
        capability = QStringLiteral("DisplayOnly");
        break;
    case DisplayYesNo:
        capability = QStringLiteral("DisplayYesNo");
        break;
    case KeyboardOnly:
        capability = QStringLiteral("KeyboardOnly");
        break;
    case NoInputNoOutput:
        capability = QStringLiteral("NoInputNoOutput");
        break;
    default:
        return;
    }

    QDBusObjectPath agentObjectPath(agentPath);
    d->m_bluezAgentManager->RegisterAgent(agentObjectPath, capability);
}

void Manager::unregisterAgent(const QString &agentPath)
{
    d->m_bluezAgentManager->UnregisterAgent(QDBusObjectPath(agentPath));
}

void Manager::requestDefaultAgent(const QString &agentPath)
{
    QDBusObjectPath agentObjectPath(agentPath);
    d->m_bluezAgentManager->RequestDefaultAgent(agentObjectPath);
}
