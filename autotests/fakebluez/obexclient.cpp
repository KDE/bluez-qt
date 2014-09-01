#include "obexclient.h"

#include <QDebug>
#include <QDBusMessage>
#include <QDBusConnection>

ObexClient::ObexClient(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    setName(QStringLiteral("org.bluez.obex.Client1"));
    setPath(QDBusObjectPath(QStringLiteral("/org/bluez/obex")));
}

void ObexClient::runAction(const QString &actionName, const QVariantMap &properties)
{
    Q_UNUSED(actionName)
    Q_UNUSED(properties)
}

void ObexClient::CreateSession(const QString &destination, const QVariantMap &args, const QDBusMessage &msg)
{
    Q_UNUSED(destination)
    Q_UNUSED(args)
    Q_UNUSED(msg)
}

void ObexClient::RemoveSession(const QDBusObjectPath &session, const QDBusMessage &msg)
{
    Q_UNUSED(session)
    Q_UNUSED(msg)
}
