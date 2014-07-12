#include "agent.h"
#include "debug_p.h"

#include <QDBusConnection>

namespace QBluez
{

// Agent
Agent::Agent(QObject *parent)
    : QObject(parent)
{
}

void Agent::requestPinCode(Device *device, const Request<QString> &request)
{
    Q_UNUSED(device)
    Q_UNUSED(request)
}

void Agent::displayPinCode(Device *device, const QString &pinCode)
{
    Q_UNUSED(device)
    Q_UNUSED(pinCode)
}

void Agent::requestPasskey(Device *device, const Request<quint32> &request)
{
    Q_UNUSED(device)
    Q_UNUSED(request)
}

void Agent::displayPasskey(Device *device, quint32 passkey, quint16 entered)
{
    Q_UNUSED(device)
    Q_UNUSED(passkey)
    Q_UNUSED(entered)
}

void Agent::requestConfirmation(Device *device, quint32 passkey, const Request<void> &request)
{
    Q_UNUSED(device)
    Q_UNUSED(passkey)
    Q_UNUSED(request)
}

void Agent::requestAuthorization(Device *device, const Request<void> &request)
{
    Q_UNUSED(device)
    Q_UNUSED(request)
}

void Agent::authorizeService(Device *device, const QString &uuid, const Request<void> &request)
{
    Q_UNUSED(device)
    Q_UNUSED(uuid)
    Q_UNUSED(request)
}

void Agent::cancel()
{
}

void Agent::release()
{
}

} // namespace QBluez
