#ifndef AGENTMANAGER_H
#define AGENTMANAGER_H

#include "object.h"

#include <QDBusAbstractAdaptor>

class AgentManager : public QDBusAbstractAdaptor, public Object
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.bluez.AgentManager1")

public:
    explicit AgentManager(QObject *parent = 0);

public Q_SLOTS:
    void RegisterAgent(const QDBusObjectPath &path, const QString &capability);
    void UnregisterAgent(const QDBusObjectPath &path);
    void RequestDefaultAgent(const QDBusObjectPath &path);
};

#endif // OBJECT_H
