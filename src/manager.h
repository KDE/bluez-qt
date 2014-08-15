#ifndef QBLUEZ_MANAGER_H
#define QBLUEZ_MANAGER_H

#include <QObject>

#include "qbluez_export.h"

namespace QBluez
{

class Device;
class Adapter;
class Agent;
class PendingCall;
class InitManagerJob;

class QBLUEZ_EXPORT Manager : public QObject
{
    Q_OBJECT

    Q_ENUMS(RegisterCapability)
    Q_PROPERTY(Adapter* usableAdapter READ usableAdapter)
    Q_PROPERTY(QList<Adapter*> adapters READ adapters)
    Q_PROPERTY(QList<Device*> devices READ devices)
    Q_PROPERTY(bool initialized READ isInitialized)
    Q_PROPERTY(bool operational READ isOperational NOTIFY operationalChanged)
    Q_PROPERTY(bool bluetoothOperational READ isBluetoothOperational)

public:
    enum RegisterCapability {
        DisplayOnly = 0,
        DisplayYesNo = 1,
        KeyboardOnly = 2,
        NoInputNoOutput = 3
    };

    explicit Manager(QObject *parent = 0);
    ~Manager();

    InitManagerJob *init();

    bool isInitialized() const;
    bool isOperational() const;
    bool isBluetoothOperational() const;

    Adapter *usableAdapter() const;
    QList<Adapter*> adapters() const;
    QList<Device*> devices() const;

    Adapter *adapterForAddress(const QString &address) const;
    Adapter *adapterForUbi(const QString &ubi) const;
    Device *deviceForAddress(const QString &address) const;
    Device *deviceForUbi(const QString &ubi) const;

    // Possible errors: InvalidArguments, AlreadyExists
    PendingCall *registerAgent(Agent *agent, RegisterCapability registerCapability = DisplayYesNo);

    // Possible errors: DoesNotExist
    PendingCall *unregisterAgent(Agent *agent);

    // Possible errors: DoesNotExist
    PendingCall *requestDefaultAgent(Agent *agent);

Q_SIGNALS:
    void operationalChanged(bool operational);
    void adapterAdded(Adapter *adapter);
    void adapterRemoved(Adapter *adapter);
    void usableAdapterChanged(Adapter *adapter);
    void allAdaptersRemoved();

private:
    class ManagerPrivate *const d;

    friend class ManagerPrivate;
    friend class InitManagerJobPrivate;
};

} // namespace QBluez

#endif // QBLUEZ_MANAGER_H
