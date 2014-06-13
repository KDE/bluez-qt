#ifndef QBLUEZ_MANAGER_P_H
#define QBLUEZ_MANAGER_P_H

#include <QObject>
#include <QHash>

#include "manager.h"
#include "dbusobjectmanager.h"
#include "bluezagentmanager1.h"

namespace QBluez
{

typedef org::freedesktop::DBus::ObjectManager DBusObjectManager;
typedef org::bluez::AgentManager1 BluezAgentManager;

class Adapter;

class ManagerPrivate : public QObject
{
    Q_OBJECT

public:
    explicit ManagerPrivate(Manager *parent);

    void initialize();
    void clear();

    // slots
    void interfacesAdded(const QDBusObjectPath &objectPath, const QVariantMapMap &interfaces);
    void interfacesRemoved(const QDBusObjectPath &objectPath, const QStringList &interfaces);

    Device *findDeviceByPath(const QString &path) const;

    Manager *q;
    DBusObjectManager *m_dbusObjectManager;
    BluezAgentManager *m_bluezAgentManager;

    QHash<QString, Adapter *> m_adapters;

    bool m_bluezRunning;
    bool m_initialized;
};

} // namespace QBluez

#endif // QBLUEZ_MANAGER_P_H
