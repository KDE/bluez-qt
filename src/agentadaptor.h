#ifndef QBLUEZ_AGENTADAPTOR_H
#define QBLUEZ_AGENTADAPTOR_H

#include <QObject>
#include <QDBusAbstractAdaptor>

#include "qbluez_export.h"

class QDBusMessage;
class QDBusObjectPath;

namespace QBluez
{

class Device;
class Manager;
class Agent;

class AgentAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.bluez.Agent1")

public:
    AgentAdaptor(Agent *parent, Manager *manager);
    ~AgentAdaptor();

public Q_SLOTS:
    QString RequestPinCode(const QDBusObjectPath &device, const QDBusMessage &msg);
    Q_NOREPLY void DisplayPinCode(const QDBusObjectPath &device, const QString &pincode);
    quint32 RequestPasskey(const QDBusObjectPath &device, const QDBusMessage &msg);
    Q_NOREPLY void DisplayPasskey(const QDBusObjectPath &device, quint32 passkey, quint8 entered);
    void RequestConfirmation(const QDBusObjectPath &device, quint32 passkey, const QDBusMessage &msg);
    void RequestAuthorization(const QDBusObjectPath &device, const QDBusMessage &msg);
    void AuthorizeService(const QDBusObjectPath &device, const QString &uuid, const QDBusMessage &msg);

    void Cancel();
    void Release();

private:
    Device *deviceForPath(const QDBusObjectPath &path) const;

    Agent *m_agent;
    Manager *m_manager;
};

} // namespace QBluez

#endif // QBLUEZ_AGENTADAPTOR_H
