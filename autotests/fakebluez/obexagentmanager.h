#ifndef OBEXAGENTMANAGER_H
#define OBEXAGENTMANAGER_H

#include "object.h"

#include <QDBusAbstractAdaptor>

class QDBusMessage;

class ObexAgentManager : public QDBusAbstractAdaptor, public Object
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.bluez.obex.AgentManager1")

public:
    explicit ObexAgentManager(QObject *parent = Q_NULLPTR);

    void runAction(const QString &actionName, const QVariantMap &properties);

public Q_SLOTS:
    void RegisterAgent(const QDBusObjectPath &path, const QDBusMessage &msg);
    void UnregisterAgent(const QDBusObjectPath &path, const QDBusMessage &msg);

private:
    void runAuthorizePushAction(const QVariantMap &properties);
    void runCancelAction();
    void runReleaseAction();

    QDBusObjectPath m_agent;
    QString m_service;
};

#endif // OBEXAGENTMANAGER_H
