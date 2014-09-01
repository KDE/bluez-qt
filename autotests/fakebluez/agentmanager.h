#ifndef AGENTMANAGER_H
#define AGENTMANAGER_H

#include "object.h"

#include <QDBusAbstractAdaptor>

class QDBusMessage;

class AgentManager : public QDBusAbstractAdaptor, public Object
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.bluez.AgentManager1")

public:
    explicit AgentManager(QObject *parent = 0);

    void runAction(const QString &actionName, const QVariantMap &properties);

public Q_SLOTS:
    void RegisterAgent(const QDBusObjectPath &path, const QString &capability, const QDBusMessage &msg);
    void UnregisterAgent(const QDBusObjectPath &path, const QDBusMessage &msg);
    void RequestDefaultAgent(const QDBusObjectPath &path);

private:
    void runRequestPinCodeAction(const QVariantMap &properties);
    void runDisplayPinCodeAction(const QVariantMap &properties);
    void runRequestPasskeyAction(const QVariantMap &properties);
    void runDisplayPasskeyAction(const QVariantMap &properties);
    void runRequestConfirmationAction(const QVariantMap &properties);
    void runRequestAuthorizationAction(const QVariantMap &properties);
    void runAuthorizeServiceAction(const QVariantMap &properties);
    void runCancelAction();
    void runReleaseAction();

    QDBusObjectPath m_agent;
    QString m_service;
    QString m_capability;
};

#endif // AGENTMANAGER_H
