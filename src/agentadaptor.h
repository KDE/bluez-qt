/*
 * BluezQt - Asynchronous Bluez wrapper library
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

#ifndef BLUEZQT_AGENTADAPTOR_H
#define BLUEZQT_AGENTADAPTOR_H

#include <QObject>
#include <QDBusAbstractAdaptor>

class QDBusMessage;
class QDBusObjectPath;

namespace BluezQt
{

class Device;
class Manager;
class Agent;

class AgentAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.bluez.Agent1")

public:
    explicit AgentAdaptor(Agent *parent, Manager *manager);

public Q_SLOTS:
    QString RequestPinCode(const QDBusObjectPath &device, const QDBusMessage &msg);
    Q_NOREPLY void DisplayPinCode(const QDBusObjectPath &device, const QString &pincode);
    quint32 RequestPasskey(const QDBusObjectPath &device, const QDBusMessage &msg);
    Q_NOREPLY void DisplayPasskey(const QDBusObjectPath &device, quint32 passkey, quint16 entered);
    void RequestConfirmation(const QDBusObjectPath &device, quint32 passkey, const QDBusMessage &msg);
    void RequestAuthorization(const QDBusObjectPath &device, const QDBusMessage &msg);
    void AuthorizeService(const QDBusObjectPath &device, const QString &uuid, const QDBusMessage &msg);

    Q_NOREPLY void Cancel();
    Q_NOREPLY void Release();

private:
    QString passkeyToString(quint32 passkey) const;

    Agent *m_agent;
    Manager *m_manager;
};

} // namespace BluezQt

#endif // BLUEZQT_AGENTADAPTOR_H
