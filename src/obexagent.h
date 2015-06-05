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

#ifndef BLUEZQT_OBEXAGENT_H
#define BLUEZQT_OBEXAGENT_H

#include <QObject>

#include "types.h"
#include "request.h"
#include "bluezqt_export.h"

class QDBusObjectPath;

namespace BluezQt
{

class Device;
class ObexTransfer;

/**
 * Bluetooth OBEX agent.
 *
 * This class represents a Bluetooth OBEX agent.
 *
 * The agent is used to authorize an incoming object push requests.
 *
 * @note The return value of request will be sent asynchronously with Request class.
 *       It is also possible to cancel/reject the request.
 */
class BLUEZQT_EXPORT ObexAgent : public QObject
{
    Q_OBJECT

public:
    /**
     * Creates a new ObexAgent object.
     *
     * @param parent
     */
    explicit ObexAgent(QObject *parent = Q_NULLPTR);

    /**
     * D-Bus object path of the agent.
     *
     * The path where the agent will be registered.
     *
     * @note You must provide valid object path!
     *
     * @return object path of agent
     */
    virtual QDBusObjectPath objectPath() const = 0;

    /**
     * Requests the agent to authorize an incoming object push request.
     *
     * This method gets called when the Bluetooth daemon
     * needs to accept/reject a Bluetooth object push request.
     *
     * The return value should be full path where the incoming object
     * will be saved.
     *
     * The ObexTransfer::fileName() contains the default location
     * and name that can be returned.
     *
     * You can use @p session to get device and adapter this transfer
     * belongs to.
     *
     * @param transfer transfer object
     * @param session transfer session
     * @param request request to be used for sending reply
     */
    virtual void authorizePush(ObexTransferPtr transfer, ObexSessionPtr session, const Request<QString> &request);

    /**
     * Indicate that the agent request failed before receiving reply.
     *
     * This method gets called to indicate that the agent
     * request failed before a reply was returned.
     *
     * It cancels the previous request.
     */
    virtual void cancel();

    /**
     * Indicates that the agent was unregistered.
     *
     * This method gets called when the Bluetooth daemon
     * unregisters the agent.
     *
     * An agent can use it to do cleanup tasks. There is no need
     * to unregister the agent, because when this method gets called
     * it has already been unregistered.
     */
    virtual void release();
};

} // namespace BluezQt

#endif // BLUEZQT_OBEXAGENT_H
