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
    explicit AgentManager(QObject *parent = Q_NULLPTR);

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
