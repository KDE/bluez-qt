#include "request.h"
#include "debug_p.h"

#include <QStringBuilder>
#include <QDBusConnection>

namespace QBluez
{

static bool sendMessage(AgentType type, const QDBusMessage &msg)
{
    if (type == PairingAgent) {
        return QDBusConnection::systemBus().send(msg);
    }
    if (type == ObexAgent) {
        return QDBusConnection::sessionBus().send(msg);
    }
    return false;
}

static QString interfaceName(AgentType type)
{
    if (type == PairingAgent) {
        return QStringLiteral("org.bluez.Agent1");
    }
    if (type == ObexAgent) {
        return QStringLiteral("org.bluez.obex.Agent1");
    }
    return QString();
}

void qbluez_acceptRequest(AgentType type, const QVariant &val, const QDBusMessage &req)
{
    QDBusMessage reply;
    if (val.isValid()) {
        reply = req.createReply(val);
    } else {
        reply = req.createReply();
    }

    if (!sendMessage(type, reply)) {
        qCWarning(QBLUEZ) << "Request: Failed to put reply on DBus queue";
    }
}

void qbluez_rejectRequest(AgentType type, const QDBusMessage &req)
{
    const QDBusMessage &reply = req.createErrorReply(interfaceName(type) % QStringLiteral(".Rejected"),
                                QStringLiteral("Rejected"));
    if (!sendMessage(type, reply)) {
        qCWarning(QBLUEZ) << "Request: Failed to put reply on DBus queue";
    }
}

void qbluez_cancelRequest(AgentType type, const QDBusMessage &req)
{
    const QDBusMessage &reply = req.createErrorReply(interfaceName(type) % QStringLiteral(".Canceled"),
                                QStringLiteral("Canceled"));
    if (!sendMessage(type, reply)) {
        qCWarning(QBLUEZ) << "Request: Failed to put reply on DBus queue";
    }
}

} // namespace QBluez
