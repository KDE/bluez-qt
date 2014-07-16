#include "agentadaptor.h"
#include "agent.h"
#include "request.h"
#include "manager.h"
#include "adapter.h"
#include "device.h"
#include "debug_p.h"

#include <QDBusConnection>
#include <QDBusObjectPath>

namespace QBluez
{

AgentAdaptor::AgentAdaptor(Agent *parent, Manager *manager)
    : QDBusAbstractAdaptor(parent)
    , m_agent(parent)
    , m_manager(manager)
    , m_iface(QStringLiteral("org.bluez.Agent1"))
{
}

AgentAdaptor::~AgentAdaptor()
{
    m_manager->unregisterAgent(m_agent);
}

QString AgentAdaptor::RequestPinCode(const QDBusObjectPath &device, const QDBusMessage &msg)
{
    msg.setDelayedReply(true);
    m_agent->requestPinCode(deviceForPath(device), Request<QString>(OrgBluezAgent, msg));
    return QString();
}

void AgentAdaptor::DisplayPinCode(const QDBusObjectPath &device, const QString &pincode)
{
    m_agent->displayPinCode(deviceForPath(device), pincode);
}

quint32 AgentAdaptor::RequestPasskey(const QDBusObjectPath &device, const QDBusMessage &msg)
{
    msg.setDelayedReply(true);
    m_agent->requestPasskey(deviceForPath(device), Request<quint32>(OrgBluezAgent, msg));
    return 0;
}

void AgentAdaptor::DisplayPasskey(const QDBusObjectPath &device, quint32 passkey, quint8 entered)
{
    m_agent->displayPasskey(deviceForPath(device), passkeyToString(passkey), QString::number(entered));
}

void AgentAdaptor::RequestConfirmation(const QDBusObjectPath &device, quint32 passkey, const QDBusMessage &msg)
{
    msg.setDelayedReply(true);
    m_agent->requestConfirmation(deviceForPath(device), passkeyToString(passkey), Request<>(OrgBluezAgent, msg));
}

void AgentAdaptor::RequestAuthorization(const QDBusObjectPath &device, const QDBusMessage &msg)
{
    msg.setDelayedReply(true);
    m_agent->requestAuthorization(deviceForPath(device), Request<>(OrgBluezAgent, msg));
}

void AgentAdaptor::AuthorizeService(const QDBusObjectPath &device, const QString &uuid, const QDBusMessage &msg)
{
    msg.setDelayedReply(true);
    m_agent->authorizeService(deviceForPath(device), uuid, Request<>(OrgBluezAgent, msg));
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
            if (device->ubi() == path.path()) {
                return device;
            }
        }
    }
    qCWarning(QBLUEZ) << "AgentAdaptor::deviceForPath Cannot find device for path:" << path.path();
    return Q_NULLPTR;
}

QString AgentAdaptor::passkeyToString(quint32 passkey) const
{
    // Passkey will always be a 6-digit number, zero-pad it at the start
    return QString(QStringLiteral("%1")).arg(passkey, 6, 10, QLatin1Char('0'));
}

} // namespace QBluez
