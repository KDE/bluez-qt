/*
 * QBluez - Asynchronous Bluez wrapper library
 *
 * Copyright (C) 2014 David Rosca <nowrep@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "request.h"
#include "debug_p.h"
#include "utils_p.h"

#include <QStringBuilder>
#include <QDBusConnection>

namespace QBluez
{

static bool sendMessage(AgentType type, const QDBusMessage &msg)
{
    if (type == OrgBluezAgent) {
        return DBusConnection::orgBluez().send(msg);
    }
    if (type == OrgBluezObexAgent) {
        return DBusConnection::orgBluezObex().send(msg);
    }
    return false;
}

static QString interfaceName(AgentType type)
{
    if (type == OrgBluezAgent) {
        return QStringLiteral("org.bluez.Agent1");
    }
    if (type == OrgBluezObexAgent) {
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
