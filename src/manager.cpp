#include "manager.h"
#include "manager_p.h"
#include "adapter.h"
#include "agent.h"
#include "agentadaptor.h"
#include "pendingcall.h"
#include "initmanagerjob.h"
#include "initadaptersjob.h"

namespace QBluez
{

Manager::Manager(QObject *parent)
    : QObject(parent)
    , d(new ManagerPrivate(this))
{
}

Manager::~Manager()
{
    delete d;
}

InitManagerJob *Manager::init(Manager::InitType type)
{
    return new InitManagerJob(this, type);
}

InitAdaptersJob *Manager::initAdapters()
{
    return new InitAdaptersJob(d);
}

bool Manager::isInitialized() const
{
    return d->m_initialized;
}

bool Manager::isOperational() const
{
    return d->m_initialized && d->m_bluezRunning && d->m_loaded;
}

bool Manager::isBluetoothOperational() const
{
    return d->m_bluezRunning && d->m_loaded && d->m_usableAdapter;
}

Adapter *Manager::usableAdapter() const
{
    return d->m_usableAdapter;
}

QList<Adapter*> Manager::adapters() const
{
    return d->m_adapters.values();
}

QList<Device*> Manager::devices() const
{
    return d->m_devices.values();
}

Adapter *Manager::adapterForAddress(const QString &address) const
{
    Q_FOREACH (Adapter *adapter, d->m_adapters) {
        if (adapter->address() == address) {
            return adapter;
        }
    }
    return Q_NULLPTR;
}

Adapter *Manager::adapterForUbi(const QString &ubi) const
{
    return d->m_adapters.value(ubi);
}

Device *Manager::deviceForAddress(const QString &address) const
{
    Q_FOREACH (Adapter *adapter, d->m_adapters) {
        Device *device = adapter->deviceForAddress(address);
        if (device) {
            return device;
        }
    }
    return Q_NULLPTR;
}

Device *Manager::deviceForUbi(const QString &ubi) const
{
    return d->m_devices.value(ubi);
}

PendingCall *Manager::registerAgent(Agent *agent, RegisterCapability registerCapability)
{
    if (!d->m_bluezAgentManager) {
        return new PendingCall(PendingCall::InternalError, QStringLiteral("Manager not operational!"));
    }

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

    return new PendingCall(d->m_bluezAgentManager->RegisterAgent(agent->objectPath(), capability),
                           PendingCall::ReturnVoid, this);
}

PendingCall *Manager::unregisterAgent(Agent *agent)
{
    if (!d->m_bluezAgentManager) {
        return new PendingCall(PendingCall::InternalError, QStringLiteral("Manager not operational!"));
    }

    QDBusConnection::systemBus().unregisterObject(agent->objectPath().path());

    return new PendingCall(d->m_bluezAgentManager->UnregisterAgent(agent->objectPath()),
                           PendingCall::ReturnVoid, this);
}

PendingCall *Manager::requestDefaultAgent(Agent *agent)
{
    if (!d->m_bluezAgentManager) {
        return new PendingCall(PendingCall::InternalError, QStringLiteral("Manager not operational!"));
    }

    return new PendingCall(d->m_bluezAgentManager->RequestDefaultAgent(agent->objectPath()),
                           PendingCall::ReturnVoid, this);
}

} // namespace QBluez
