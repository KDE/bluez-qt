#ifndef QBLUEZ_MANAGER_H
#define QBLUEZ_MANAGER_H

#include <QObject>

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
    Q_PROPERTY(bool operational READ isOperational NOTIFY operationalChanged)

public:
    enum RegisterCapability {
        DisplayOnly = 0,
        DisplayYesNo = 1,
        KeyboardOnly = 2,
        NoInputNoOutput = 3
    };

    static Manager *self();
    static void release();

    QList<Adapter *> adapters() const;
    QList<Device *> devices() const;

    bool isOperational() const;

    void registerAgent(const QString &agentPath, RegisterCapability registerCapability);
    void unregisterAgent(const QString &agentPath);
    void requestDefaultAgent(const QString &agentPath);

Q_SIGNALS:
    void operationalChanged(bool operational);

    void adapterAdded(Adapter *adapter);
    void adapterRemoved(Adapter *adapter);
    void usableAdapterChanged(Adapter *adapter);

private:
    explicit Manager();
    ~Manager();

    ManagerPrivate *const d;
    friend class ManagerPrivate;
};

} // namespace QBluez

#endif // QBLUEZ_MANAGER_H
