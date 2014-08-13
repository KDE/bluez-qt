#include "agentadaptor.h"
#include "agent.h"
#include "manager.h"
#include "adapter.h"
#include "device.h"
#include "loaddevicejob.h"

#include <QDBusConnection>
#include <QDBusObjectPath>

namespace QBluez
{

AgentAdaptor::AgentAdaptor(Agent *parent, Manager *manager)
    : QDBusAbstractAdaptor(parent)
    , m_agent(parent)
    , m_manager(manager)
    , m_passKey(0)
    , m_enteredPassKey(0)
{
}

QString AgentAdaptor::RequestPinCode(const QDBusObjectPath &device, const QDBusMessage &msg)
{
    msg.setDelayedReply(true);
    m_stringRequest = Request<QString>(OrgBluezAgent, msg);

    Device *dev = m_manager->deviceForUbi(device.path());
    if (!dev) {
        m_stringRequest.cancel();
        return QString();
    }

    LoadDeviceJob *job = dev->load();
    job->start();
    connect(job, &LoadDeviceJob::result, this, &AgentAdaptor::finishRequestPinCode);

    return QString();
}

void AgentAdaptor::DisplayPinCode(const QDBusObjectPath &device, const QString &pincode)
{
    Device *dev = m_manager->deviceForUbi(device.path());
    if (!dev) {
        return;
    }

    m_pinCode = pincode;

    LoadDeviceJob *job = dev->load();
    job->start();
    connect(job, &LoadDeviceJob::result, this, &AgentAdaptor::finishDisplayPinCode);
}

quint32 AgentAdaptor::RequestPasskey(const QDBusObjectPath &device, const QDBusMessage &msg)
{
    msg.setDelayedReply(true);
    m_uintRequest = Request<quint32>(OrgBluezAgent, msg);

    Device *dev = m_manager->deviceForUbi(device.path());
    if (!dev) {
        m_uintRequest.cancel();
        return 0;
    }

    LoadDeviceJob *job = dev->load();
    job->start();
    connect(job, &LoadDeviceJob::result, this, &AgentAdaptor::finishRequestPasskey);

    return 0;
}

void AgentAdaptor::DisplayPasskey(const QDBusObjectPath &device, quint32 passkey, quint8 entered)
{
    Device *dev = m_manager->deviceForUbi(device.path());
    if (!dev) {
        return;
    }

    m_passKey = passkey;
    m_enteredPassKey = entered;

    LoadDeviceJob *job = dev->load();
    job->start();
    connect(job, &LoadDeviceJob::result, this, &AgentAdaptor::finishDisplayPasskey);
}

void AgentAdaptor::RequestConfirmation(const QDBusObjectPath &device, quint32 passkey, const QDBusMessage &msg)
{
    msg.setDelayedReply(true);
    m_voidRequest = Request<>(OrgBluezAgent, msg);

    Device *dev = m_manager->deviceForUbi(device.path());
    if (!dev) {
        m_voidRequest.cancel();
        return;
    }

    m_passKey = passkey;

    LoadDeviceJob *job = dev->load();
    job->start();
    connect(job, &LoadDeviceJob::result, this, &AgentAdaptor::finishRequestConfirmation);
}

void AgentAdaptor::RequestAuthorization(const QDBusObjectPath &device, const QDBusMessage &msg)
{
    msg.setDelayedReply(true);
    m_voidRequest = Request<>(OrgBluezAgent, msg);

    Device *dev = m_manager->deviceForUbi(device.path());
    if (!dev) {
        m_voidRequest.cancel();
        return;
    }

    LoadDeviceJob *job = dev->load();
    job->start();
    connect(job, &LoadDeviceJob::result, this, &AgentAdaptor::finishRequestAuthorization);
}

void AgentAdaptor::AuthorizeService(const QDBusObjectPath &device, const QString &uuid, const QDBusMessage &msg)
{
    msg.setDelayedReply(true);
    m_voidRequest = Request<>(OrgBluezAgent, msg);

    Device *dev = m_manager->deviceForUbi(device.path());
    if (!dev) {
        m_voidRequest.cancel();
        return;
    }

    m_uuid = uuid;

    LoadDeviceJob *job = dev->load();
    job->start();
    connect(job, &LoadDeviceJob::result, this, &AgentAdaptor::finishAuthorizeService);
}

void AgentAdaptor::Cancel()
{
    m_agent->cancel();
}

void AgentAdaptor::Release()
{
    m_agent->release();
}

void AgentAdaptor::finishRequestPinCode(LoadDeviceJob *job)
{
    Q_ASSERT(!job->error());
    if (job->error()) {
        m_stringRequest.cancel();
        return;
    }
    m_agent->requestPinCode(job->device(), m_stringRequest);
}

void AgentAdaptor::finishDisplayPinCode(LoadDeviceJob *job)
{
    Q_ASSERT(!job->error());
    if (job->error()) {
        return;
    }
    m_agent->displayPinCode(job->device(), m_pinCode);
}

void AgentAdaptor::finishRequestPasskey(LoadDeviceJob *job)
{
    Q_ASSERT(!job->error());
    if (job->error()) {
        m_uintRequest.cancel();
        return;
    }
    m_agent->requestPasskey(job->device(), m_uintRequest);
}

void AgentAdaptor::finishDisplayPasskey(LoadDeviceJob *job)
{
    Q_ASSERT(!job->error());
    if (job->error()) {
        return;
    }
    m_agent->displayPasskey(job->device(), passkeyToString(m_passKey), QString::number(m_enteredPassKey));
}

void AgentAdaptor::finishRequestConfirmation(LoadDeviceJob *job)
{
    Q_ASSERT(!job->error());
    if (job->error()) {
        m_voidRequest.cancel();
        return;
    }
    m_agent->requestConfirmation(job->device(), passkeyToString(m_passKey), m_voidRequest);
}

void AgentAdaptor::finishRequestAuthorization(LoadDeviceJob *job)
{
    Q_ASSERT(!job->error());
    if (job->error()) {
        m_voidRequest.cancel();
        return;
    }
    m_agent->requestAuthorization(job->device(), m_voidRequest);
}

void AgentAdaptor::finishAuthorizeService(LoadDeviceJob *job)
{
    Q_ASSERT(!job->error());
    if (job->error()) {
        m_voidRequest.cancel();
        return;
    }
    m_agent->authorizeService(job->device(), m_uuid, m_voidRequest);
}

QString AgentAdaptor::passkeyToString(quint32 passkey) const
{
    // Passkey will always be a 6-digit number, zero-pad it at the start
    return QString(QStringLiteral("%1")).arg(passkey, 6, 10, QLatin1Char('0'));
}

} // namespace QBluez
