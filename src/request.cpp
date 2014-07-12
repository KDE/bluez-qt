#include "request.h"
#include "debug_p.h"

#include <QStringBuilder>
#include <QDBusConnection>

namespace QBluez
{

void qbluez_acceptRequest(const QDBusMessage &req, const QVariant &val)
{
    QDBusMessage reply;
    if (val.isValid()) {
        reply = req.createReply(val);
    } else {
        reply = req.createReply();
    }

    if (!QDBusConnection::systemBus().send(reply)) {
        qCWarning(QBLUEZ) << "Request: Failed to put reply on DBus queue";
    }
}

void qbluez_rejectRequest(const QDBusMessage &req, const QString &iface)
{
    const QDBusMessage &reply = req.createErrorReply(iface % QStringLiteral(".Rejected"),
                                                     QStringLiteral("Rejected"));
    if (!QDBusConnection::systemBus().send(reply)) {
        qCWarning(QBLUEZ) << "Request: Failed to put reply on DBus queue";
    }
}

void qbluez_cancelRequest(const QDBusMessage &req, const QString &iface)
{
    const QDBusMessage &reply = req.createErrorReply(iface % QStringLiteral(".Canceled"),
                                                     QStringLiteral("Canceled"));
    if (!QDBusConnection::systemBus().send(reply)) {
        qCWarning(QBLUEZ) << "Request: Failed to put reply on DBus queue";
    }
}

} // namespace QBluez
