#ifndef QBLUEZ_MANAGER_P_H
#define QBLUEZ_MANAGER_P_H

#include <QObject>
#include <QHash>

#include "dbusobjectmanager.h"
#include "bluezagentmanager1.h"

namespace QBluez
{

typedef org::freedesktop::DBus::ObjectManager DBusObjectManager;
typedef org::bluez::AgentManager1 BluezAgentManager;

class Manager;
class Adapter;
class Device;
class AdapterPrivate;

class ManagerPrivate : public QObject
{
    Q_OBJECT

public:
    explicit ManagerPrivate(Manager *parent);
    ~ManagerPrivate();

    void init();
    void nameHasOwnerFinished(QDBusPendingCallWatcher *watcher);
    void load();
    void getManagedObjectsFinished(QDBusPendingCallWatcher *watcher);
    void clear();

    Adapter *findUsableAdapter() const;

    void serviceRegistered();
    void serviceUnregistered();
    void interfacesAdded(const QDBusObjectPath &objectPath, const QVariantMapMap &interfaces);
    void interfacesRemoved(const QDBusObjectPath &objectPath, const QStringList &interfaces);
    void adapterLoaded(AdapterPrivate *adapter);

    void adapterRemoved(Adapter *adapter);
    void adapterPoweredChanged(bool powered);

    Manager *q;
    DBusObjectManager *m_dbusObjectManager;
    BluezAgentManager *m_bluezAgentManager;

    QHash<QString, Adapter*> m_adapters;
    QHash<QString, Device*> m_devices;
    Adapter *m_usableAdapter;

    bool m_initialized;
    bool m_bluezRunning;
    bool m_loaded;
    bool m_adaptersLoaded;

Q_SIGNALS:
    void initError(const QString &errorText);
    void initFinished();
};

} // namespace QBluez

#endif // QBLUEZ_MANAGER_P_H
