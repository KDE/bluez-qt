#include "manager.h"
#include "manager_p.h"
#include "adapter.h"
#include "loadadaptersjob.h"
#include "agent.h"
#include "agentadaptor.h"

namespace QBluez
{

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
GetManagerJob *Manager::get()
{
    return new GetManagerJob(instance, qApp);
}

void Manager::release()
{
    instance->deleteLater();
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

bool Manager::isBluetoothOperational() const
{
    return d->m_bluezRunning && d->m_initialized && d->usableAdapter();
}

PendingCall *Manager::registerAgent(Agent *agent, RegisterCapability registerCapability)
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
        capability = QStringLiteral("DisplayYesNo");
        break;
    }

    new AgentAdaptor(agent, this);

    if (!QDBusConnection::systemBus().registerObject(agent->objectPath().path(), agent)) {
        qWarning() << "Cannot register object" << agent->objectPath().path();
    }

    return new PendingCall(d->m_bluezAgentManager->RegisterAgent(agent->objectPath(), capability), this);
}

PendingCall *Manager::unregisterAgent(Agent *agent)
{
    return new PendingCall(d->m_bluezAgentManager->UnregisterAgent(agent->objectPath()), this);
}

PendingCall *Manager::requestDefaultAgent(Agent *agent)
{
    return new PendingCall(d->m_bluezAgentManager->RequestDefaultAgent(agent->objectPath()), this);
}

// static
Manager *Manager::self()
{
    if (!instance) {
        instance = new Manager();
    }
    return instance;
}

} // namespace QBluez
