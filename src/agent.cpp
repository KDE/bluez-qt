#include "agent.h"

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

void Agent::displayPasskey(Device *device, const QString &passkey, const QString &entered)
{
    Q_UNUSED(device)
    Q_UNUSED(passkey)
    Q_UNUSED(entered)
}

void Agent::requestConfirmation(Device *device, const QString &passkey, const Request<> &request)
{
    Q_UNUSED(device)
    Q_UNUSED(passkey)
    Q_UNUSED(request)
}

void Agent::requestAuthorization(Device *device, const Request<> &request)
{
    Q_UNUSED(device)
    Q_UNUSED(request)
}

void Agent::authorizeService(Device *device, const QString &uuid, const Request<> &request)
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
