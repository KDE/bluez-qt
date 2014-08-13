#ifndef QBLUEZ_AGENTADAPTOR_H
#define QBLUEZ_AGENTADAPTOR_H

#include <QObject>
#include <QDBusAbstractAdaptor>

#include "request.h"

class QDBusMessage;
class QDBusObjectPath;

namespace QBluez
{

class Device;
class Manager;
class Agent;
class LoadDeviceJob;

class AgentAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.bluez.Agent1")

public:
    explicit AgentAdaptor(Agent *parent, Manager *manager);

public Q_SLOTS:
    QString RequestPinCode(const QDBusObjectPath &device, const QDBusMessage &msg);
    Q_NOREPLY void DisplayPinCode(const QDBusObjectPath &device, const QString &pincode);
    quint32 RequestPasskey(const QDBusObjectPath &device, const QDBusMessage &msg);
    Q_NOREPLY void DisplayPasskey(const QDBusObjectPath &device, quint32 passkey, quint8 entered);
    void RequestConfirmation(const QDBusObjectPath &device, quint32 passkey, const QDBusMessage &msg);
    void RequestAuthorization(const QDBusObjectPath &device, const QDBusMessage &msg);
    void AuthorizeService(const QDBusObjectPath &device, const QString &uuid, const QDBusMessage &msg);

    Q_NOREPLY void Cancel();
    Q_NOREPLY void Release();

private:
    void finishRequestPinCode(LoadDeviceJob *job);
    void finishDisplayPinCode(LoadDeviceJob *job);
    void finishRequestPasskey(LoadDeviceJob *job);
    void finishDisplayPasskey(LoadDeviceJob *job);
    void finishRequestConfirmation(LoadDeviceJob *job);
    void finishRequestAuthorization(LoadDeviceJob *job);
    void finishAuthorizeService(LoadDeviceJob *job);

    Device *deviceForPath(const QDBusObjectPath &path) const;
    QString passkeyToString(quint32 passkey) const;

    Agent *m_agent;
    Manager *m_manager;
    Request<QString> m_stringRequest;
    Request<quint32> m_uintRequest;
    Request<> m_voidRequest;
    QString m_pinCode;
    quint32 m_passKey;
    quint32 m_enteredPassKey;
    QString m_uuid;
};

} // namespace QBluez

#endif // QBLUEZ_AGENTADAPTOR_H
