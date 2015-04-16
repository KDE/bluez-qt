/*
 * Copyright (C) 2014-2015 David Rosca <nowrep@gmail.com>
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

#include "obexagentmanager.h"

#include <QDebug>
#include <QDBusMessage>
#include <QDBusConnection>

ObexAgentManager::ObexAgentManager(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    setName(QStringLiteral("org.bluez.obex.AgentManager1"));
    setPath(QDBusObjectPath(QStringLiteral("/org/bluez/obex")));
}

void ObexAgentManager::runAction(const QString &actionName, const QVariantMap &properties)
{
    if (actionName == QLatin1String("authorize-push")) {
        runAuthorizePushAction(properties);
    }
}

void ObexAgentManager::RegisterAgent(const QDBusObjectPath &path, const QDBusMessage &msg)
{
    m_agent = path;
    m_service = msg.service();
}

void ObexAgentManager::UnregisterAgent(const QDBusObjectPath &path, const QDBusMessage &msg)
{
    if (m_agent == path && m_service == msg.service()) {
        m_agent = QDBusObjectPath();
        m_service.clear();
    }
}

void ObexAgentManager::runAuthorizePushAction(const QVariantMap &properties)
{
    Q_UNUSED(properties)

    QDBusMessage call = QDBusMessage::createMethodCall(m_service,
                        m_agent.path(),
                        QStringLiteral("org.bluez.obex.Agent1"),
                        QStringLiteral("AuthorizePush"));

    call << QVariant::fromValue(QDBusObjectPath(QStringLiteral("/org/bluez/session1/transfer1")));
    const QString &fileName = QDBusConnection::sessionBus().call(call).arguments().first().toString();
    Q_UNUSED(fileName)
}

void ObexAgentManager::runCancelAction()
{
    QDBusMessage call = QDBusMessage::createMethodCall(m_service,
                        m_agent.path(),
                        QStringLiteral("org.bluez.obex.Agent1"),
                        QStringLiteral("Cancel"));
    QDBusConnection::sessionBus().call(call);
}

void ObexAgentManager::runReleaseAction()
{
    QDBusMessage call = QDBusMessage::createMethodCall(m_service,
                        m_agent.path(),
                        QStringLiteral("org.bluez.obex.Agent1"),
                        QStringLiteral("Release"));
    QDBusConnection::sessionBus().call(call);
}
