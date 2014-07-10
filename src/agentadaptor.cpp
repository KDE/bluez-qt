#include "agentadaptor.h"
#include "agent.h"
#include "manager.h"
#include "adapter.h"
#include "device.h"

#include <QDBusObjectPath>
#include <QDebug>

namespace QBluez
{

AgentAdaptor::AgentAdaptor(Agent *parent, Manager *manager)
    : QDBusAbstractAdaptor(parent)
    , m_agent(parent)
    , m_manager(manager)
{
}

AgentAdaptor::~AgentAdaptor()
{
    m_manager->unregisterAgent(m_agent);
}

QString AgentAdaptor::RequestPinCode(const QDBusObjectPath &device, const QDBusMessage &msg)
{
    return m_agent->requestPinCode(deviceForPath(device), msg);
}

void AgentAdaptor::DisplayPinCode(const QDBusObjectPath &device, const QString &pincode)
{
    m_agent->displayPinCode(deviceForPath(device), pincode);
}

quint32 AgentAdaptor::RequestPasskey(const QDBusObjectPath &device, const QDBusMessage &msg)
{
    return m_agent->requestPasskey(deviceForPath(device), msg);
}

void AgentAdaptor::DisplayPasskey(const QDBusObjectPath &device, quint32 passkey, quint8 entered)
{
    m_agent->displayPasskey(deviceForPath(device), passkey, entered);
}

void AgentAdaptor::RequestConfirmation(const QDBusObjectPath &device, quint32 passkey, const QDBusMessage &msg)
{
    m_agent->requestConfirmation(deviceForPath(device), passkey, msg);
}

void AgentAdaptor::RequestAuthorization(const QDBusObjectPath &device, const QDBusMessage &msg)
{
    m_agent->requestAuthorization(deviceForPath(device), msg);
}

void AgentAdaptor::AuthorizeService(const QDBusObjectPath &device, const QString &uuid, const QDBusMessage &msg)
{
    m_agent->authorizeService(deviceForPath(device), uuid, msg);
}

void AgentAdaptor::Cancel()
{
    m_agent->cancel();
}

void AgentAdaptor::Release()
{
    m_agent->release();
}

Device *AgentAdaptor::deviceForPath(const QDBusObjectPath &path) const
{
    Q_FOREACH (Adapter *adapter, m_manager->adapters()) {
        Q_FOREACH (Device *device, adapter->devices()) {
            if (device->path() == path.path()) {
                return device;
            }
        }
    }
    qWarning() << "Cannot find device for path:" << path.path();
    return 0;
}

} // namespace QBluez
