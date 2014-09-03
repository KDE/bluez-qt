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
    Q_PROPERTY(QBluez::Adapter* usableAdapter READ usableAdapter NOTIFY usableAdapterChanged)
    Q_PROPERTY(QList<QBluez::Adapter*> adapters READ adapters)
    Q_PROPERTY(QList<QBluez::Device*> devices READ devices)
    Q_PROPERTY(bool initialized READ isInitialized)
    Q_PROPERTY(bool operational READ isOperational NOTIFY operationalChanged)
    Q_PROPERTY(bool bluetoothOperational READ isBluetoothOperational NOTIFY bluetoothOperationalChanged)

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

public Q_SLOTS:
    QBluez::Adapter *adapterForAddress(const QString &address) const;
    QBluez::Adapter *adapterForUbi(const QString &ubi) const;
    QBluez::Device *deviceForAddress(const QString &address) const;
    QBluez::Device *deviceForUbi(const QString &ubi) const;

    // Possible errors: InvalidArguments, AlreadyExists
    PendingCall *registerAgent(Agent *agent, RegisterCapability registerCapability = DisplayYesNo);

    // Possible errors: DoesNotExist
    PendingCall *unregisterAgent(Agent *agent);

    // Possible errors: DoesNotExist
    PendingCall *requestDefaultAgent(Agent *agent);

Q_SIGNALS:
    void operationalChanged(bool operational);
    void bluetoothOperationalChanged(bool operational);
    void adapterAdded(QBluez::Adapter *adapter);
    void adapterRemoved(QBluez::Adapter *adapter);
    void usableAdapterChanged(QBluez::Adapter *adapter);
    void allAdaptersRemoved();

private:
    class ManagerPrivate *const d;

    friend class ManagerPrivate;
    friend class InitManagerJobPrivate;
};

} // namespace QBluez

#endif // QBLUEZ_MANAGER_H
