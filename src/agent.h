#ifndef QBLUEZ_AGENT_H
#define QBLUEZ_AGENT_H

#include <QObject>
#include <QDBusMessage>

#include "request.h"
#include "qbluez_export.h"

class QDBusObjectPath;

namespace QBluez
{

class Device;

class QBLUEZ_EXPORT Agent : public QObject
{
    Q_OBJECT

public:
    explicit Agent(QObject *parent = 0);

    virtual QDBusObjectPath objectPath() const = 0;

public Q_SLOTS:
    virtual void requestPinCode(Device *device, const Request<QString> &request);
    virtual void displayPinCode(Device *device, const QString &pinCode);
    virtual void requestPasskey(Device *device, const Request<quint32> &request);
    virtual void displayPasskey(Device *device, const QString &passkey, const QString &entered);
    virtual void requestConfirmation(Device *device, const QString &passkey, const Request<> &request);
    virtual void requestAuthorization(Device *device, const Request<> &request);
    virtual void authorizeService(Device *device, const QString &uuid, const Request<> &request);

    virtual void cancel();
    virtual void release();
};

} // namespace QBluez

#endif // QBLUEZ_AGENT_H
