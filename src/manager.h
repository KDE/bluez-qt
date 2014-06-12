#ifndef QBLUEZ_MANAGER_H
#define QBLUEZ_MANAGER_H

#include <QObject>

#include "qbluez_export.h"

namespace QBluez
{

class DeviceInfo;
class AdapterInfo;
class ManagerPrivate;

class QBLUEZ_EXPORT Manager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(Manager* self READ self)
    Q_PROPERTY(QList<AdapterInfo *> adapters READ adapters)
    Q_PROPERTY(QList<DeviceInfo *> devices READ devices)
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

    QList<AdapterInfo *> adapters() const;
    QList<DeviceInfo *> devices() const;

    bool isOperational() const;

    void registerAgent(const QString &agentPath, RegisterCapability registerCapability);
    void unregisterAgent(const QString &agentPath);
    void requestDefaultAgent(const QString& agentPath);

Q_SIGNALS:
    void operationalChanged(bool operational);

    void adapterAdded(AdapterInfo *adapter);
    void adapterRemoved(AdapterInfo *adapter);

private:
    explicit Manager();
    ~Manager();

    ManagerPrivate *const d;
    friend class ManagerPrivate;
};

} // namespace QBluez

#endif // QBLUEZ_MANAGER_H
