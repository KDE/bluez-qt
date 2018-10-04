/*
 * BluezQt - Asynchronous BlueZ wrapper library
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

#include <QSharedPointer>

#include "bluezqt_export.h"

class QDBusMessage;

namespace BluezQt
{

enum RequestOriginatingType {
    OrgBluezAgent,
    OrgBluezProfile,
    OrgBluezObexAgent,
    OrgBluezMediaEndpoint
};

/**
 * @class BluezQt::Request request.h <BluezQt/Request>
 *
 * D-Bus request.
 *
 * This class represents a request from a Bluetooth daemon. It is a convenient
 * wrapper around QDBusMessage and easily allows sending replies and handling errors.
 *
 * @see Agent, ObexAgent, Profile
 */
template<typename T = void>
class BLUEZQT_EXPORT Request
{
public:
    /**
     * Creates a new Request object.
     */
    explicit Request();

    /**
     * Destroys a Request object.
     */
    virtual ~Request();

    /**
     * Copy constructor.
     *
     * @param other
     */
    Request(const Request &other);

    /**
     * Copy assignment operator.
     *
     * @param other
     */
    Request &operator=(const Request &other);

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
    void accept(T returnValue) const;

    /**
     * Rejects the request.
     *
     * This method should be called to send an error reply to
     * indicate the request was rejected.
     */
    void reject() const;

    /**
     * Cancels the request.
     *
     * This method should be called to send an error reply to
     * indicate the request was canceled.
     */
    void cancel() const;

private:
    explicit Request(RequestOriginatingType type, const QDBusMessage &message);

    QSharedPointer<class RequestPrivate> d;

    friend class AgentAdaptor;
    friend class ObexAgentAdaptor;
    friend class ProfileAdaptor;
    friend class MediaEndpointAdaptor;
};

// void
template<>
class BLUEZQT_EXPORT Request<void>
{
public:
    explicit Request();
    virtual ~Request();

    Request(const Request &other);
    Request &operator=(const Request &other);

    void accept() const;
    void reject() const;
    void cancel() const;

private:
    explicit Request(RequestOriginatingType type, const QDBusMessage &message);

    QSharedPointer<class RequestPrivate> d;

    friend class AgentAdaptor;
    friend class ObexAgentAdaptor;
    friend class ProfileAdaptor;
};

} // namespace BluezQt

#endif // BLUEZQT_REQUEST_H
