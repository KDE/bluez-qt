#include "agent.h"

namespace QBluez
{

Agent::Agent(QObject *parent)
    : QObject(parent)
{
}

QString Agent::requestPinCode(Device *device, const QDBusMessage &message)
{
    Q_UNUSED(device)
    Q_UNUSED(message)
    return QString();
}

void Agent::displayPinCode(Device *device, const QString &pinCode)
{
    Q_UNUSED(device)
    Q_UNUSED(pinCode)
}

quint32 Agent::requestPasskey(Device *device, const QDBusMessage &message)
{
    Q_UNUSED(device)
    Q_UNUSED(message)
    return 0;
}

void Agent::displayPasskey(Device *device, quint32 passkey, quint16 entered)
{
    Q_UNUSED(device)
    Q_UNUSED(passkey)
    Q_UNUSED(entered)
}

void Agent::requestConfirmation(Device *device, quint32 passkey, const QDBusMessage &message)
{
    Q_UNUSED(device)
    Q_UNUSED(passkey)
    Q_UNUSED(message)
}

void Agent::requestAuthorization(Device *device, const QDBusMessage &message)
{
    Q_UNUSED(device)
    Q_UNUSED(message)
}

void Agent::authorizeService(Device *device, const QString &uuid, const QDBusMessage &message)
{
    Q_UNUSED(device)
    Q_UNUSED(uuid)
    Q_UNUSED(message)
}

void Agent::cancel()
{
}

void Agent::release()
{
}

} // namespace QBluez
