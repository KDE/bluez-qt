/*
 * BluezQt - Asynchronous Bluez wrapper library
 *
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

#ifndef BLUEZQT_REQUEST_H
#define BLUEZQT_REQUEST_H

#include <QDBusMessage>

#include <bluezqt_export.h>

namespace BluezQt
{

enum AgentType {
    OrgBluezAgent,
    OrgBluezProfile,
    OrgBluezObexAgent
};

BLUEZQT_EXPORT void bluezqt_acceptRequest(AgentType type, const QVariant &val, const QDBusMessage &req);
BLUEZQT_EXPORT void bluezqt_rejectRequest(AgentType type, const QDBusMessage &req);
BLUEZQT_EXPORT void bluezqt_cancelRequest(AgentType type, const QDBusMessage &req);

/**
 * ObexAgent/Agent request.
 *
 * This class represents a request from an agent. It is a convenient wrapper around
 * QDBusMessage and easily allows sending replies and handling errors.
 */
template<typename T = void>
class Request
{
public:
    /**
     * Creates a new Request object.
     */
    Request()
    {
    }

    /**
     * Creates a new Request object.
     *
     * @param type type of agent
     * @param message message used for reply
     */
    Request(AgentType type, const QDBusMessage &message)
        : m_type(type)
        , m_message(message)
    {
    }

    /**
     * Accepts the request.
     *
     * This method should be called to send a reply to indicate
     * the request was accepted.
     *
     * In case the request is of type void, this method does not
     * take any parameter.
     *
     * @param returnValue return value of request
     */
    void accept(T returnValue) const
    {
        bluezqt_acceptRequest(m_type, returnValue, m_message);
    }

    /**
     * Rejects the request.
     *
     * This method should be called to send an error reply to
     * indicate the request was rejected.
     */
    void reject() const
    {
        bluezqt_rejectRequest(m_type, m_message);
    }

    /**
     * Cancels the request.
     *
     * This method should be called to send an error reply to
     * indicate the request was canceled.
     */
    void cancel() const
    {
        bluezqt_cancelRequest(m_type, m_message);
    }

private:
    AgentType m_type;
    QDBusMessage m_message;
};

// void
template<>
class Request<void>
{
public:
    Request()
    {
    }

    Request(AgentType type, const QDBusMessage &message)
        : m_type(type)
        , m_message(message)
    {
    }

    void accept() const
    {
        bluezqt_acceptRequest(m_type, QVariant(), m_message);
    }

    void reject() const
    {
        bluezqt_rejectRequest(m_type, m_message);
    }

    void cancel() const
    {
        bluezqt_cancelRequest(m_type, m_message);
    }

private:
    AgentType m_type;
    QDBusMessage m_message;
};

} // namespace BluezQt

#endif // BLUEZQT_REQUEST_H
