#ifndef QBLUEZ_AGENT_H
#define QBLUEZ_AGENT_H

#include <QObject>
#include <QDBusContext>

#include "qbluez_export.h"

class QDBusObjectPath;

namespace QBluez
{

class Device;

class QBLUEZ_EXPORT Agent : public QObject
{
    Q_OBJECT

public:
    explicit Agent(QObject *parent);

    virtual QDBusObjectPath objectPath() const = 0;

public Q_SLOTS:
    virtual QString requestPinCode(Device *device, const QDBusMessage &message);
    virtual void displayPinCode(Device *device, const QString &pinCode);
    virtual quint32 requestPasskey(Device *device, const QDBusMessage &message);
    virtual void displayPasskey(Device *device, quint32 passkey, quint16 entered);
    virtual void requestConfirmation(Device *device, quint32 passkey, const QDBusMessage &message);
    virtual void requestAuthorization(Device *device, const QDBusMessage &message);
    virtual void authorizeService(Device *device, const QString &uuid, const QDBusMessage &message);

    virtual void cancel();
    virtual void release();
};

} // namespace QBluez

#endif // QBLUEZ_AGENT_H
