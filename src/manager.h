#ifndef QBLUEZ_MANAGER_H
#define QBLUEZ_MANAGER_H

#include <QObject>

#include "getmanagerjob.h"
#include "loadadaptersjob.h"
#include "qbluez_export.h"

namespace QBluez
{

class Device;
class Adapter;
class ManagerPrivate;

class QBLUEZ_EXPORT Manager : public QObject
{
    Q_OBJECT

    Q_ENUMS(RegisterCapability)
    Q_PROPERTY(Manager* self READ self)
    Q_PROPERTY(QList<Adapter *> adapters READ adapters)
    Q_PROPERTY(QList<Device *> devices READ devices)
    Q_PROPERTY(bool bluetoothOperational READ isBluetoothOperational)

public:
    enum RegisterCapability {
        DisplayOnly = 0,
        DisplayYesNo = 1,
        KeyboardOnly = 2,
        NoInputNoOutput = 3
    };

    static GetManagerJob *get();
    void release();

    QList<Adapter *> adapters() const;
    QList<Device *> devices() const;

    LoadAdaptersJob *loadAdapters();

    Adapter *usableAdapter();

    bool isBluetoothOperational() const;

    void registerAgent(const QString &agentPath, RegisterCapability registerCapability);
    void unregisterAgent(const QString &agentPath);
    void requestDefaultAgent(const QString &agentPath);

Q_SIGNALS:
    void adapterAdded(Adapter *adapter);
    void adapterRemoved(Adapter *adapter);
    void usableAdapterChanged(Adapter *adapter);
    void allAdaptersRemoved();

private:
    explicit Manager();
    ~Manager();

    static Manager *self();

    ManagerPrivate *const d;
    friend class ManagerPrivate;
    friend class GetManagerJobPrivate;
};

} // namespace QBluez

#endif // QBLUEZ_MANAGER_H
