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

#ifndef BLUEZQT_AGENT_H
#define BLUEZQT_AGENT_H

#include <QObject>

#include "request.h"
#include "types.h"
#include "bluezqt_export.h"

class QDBusObjectPath;

namespace BluezQt
{

class Device;

/**
 * Bluetooth agent.
 *
 * This class represents a Bluetooth agent.
 *
 * The agent is used in pairing process to do various actions.
 *
 * @note The return value of requests will be sent asynchronously with Request class.
 *       It is also possible to cancel/reject all requests.
 */
class BLUEZQT_EXPORT Agent : public QObject
{
    Q_OBJECT

    Q_ENUMS(Capability)
    Q_PROPERTY(Capability capability READ capability)

public:
    /**
     * The input/output capabilities of Agent.
     */
    enum Capability {
        DisplayOnly = 0,
        DisplayYesNo = 1,
        KeyboardOnly = 2,
        NoInputNoOutput = 3
    };

    /**
     * Creates a new Agent object.
     *
     * @param parent
     */
    explicit Agent(QObject *parent = Q_NULLPTR);

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
     * Input/output capability of the agent.
     *
     * By default, this method returns DisplayYesNo.
     *
     * @return capability of agent
     */
    virtual Capability capability() const;

    /**
     * Requests a PIN code from the agent.
     *
     * This method gets called when the Bluetooth daemon
     * needs to get the PIN code for an authentication.
     *
     * The return value should be a string of 1-16 characters
     * length. The string can be alphanumeric.
     *
     * @param device device that invoked the action
     * @param request request to be used for sending reply
     */
    virtual void requestPinCode(DevicePtr device, const Request<QString> &request);

    /**
     * Requests the agent to display a PIN code.
     *
     * This method gets called when the Bluetooth daemon
     * needs to display a pincode for an authentication.
     *
     * When the PIN code needs no longer to be displayed,
     * the cancel() method will be called.
     *
     * @param device device that invoked the action
     * @param pinCode PIN code to be displayed
     */
    virtual void displayPinCode(DevicePtr device, const QString &pinCode);

    /**
     * Requests a passkey from the agent.
     *
     * This method gets called when the Bluetooth daemon
     * needs to get the passkey for an authentication.
     *
     * The return value should be a numeric value between 0-999999.
     *
     * @param device device that invoked the action
     * @param request request to be used for sending reply
     */
    virtual void requestPasskey(DevicePtr device, const Request<quint32> &request);

    /**
     * Requests the agent to display a passkey.
     *
     * This method gets called when the Bluetooth daemon
     * needs to display a passkey for an authentication.
     *
     * When the passkey needs no longer to be displayed,
     * the cancel() method will be called.
     *
     * @param device device that invoked the action
     * @param passkey passkey to be displayed
     * @param entered number of already typed keys on the remote side
     */
    virtual void displayPasskey(DevicePtr device, const QString &passkey, const QString &entered);

    /**
     * Requests the agent to confirm a passkey.
     *
     * This method gets called when the Bluetooth daemon
     * needs to confirm a passkey for an authentication.
     *
     * @param device device that invoked the action
     * @param passkey passkey to be confirmed
     * @param request request to be used for sending reply
     */
    virtual void requestConfirmation(DevicePtr device, const QString &passkey, const Request<> &request);

    /**
     * Requests the agent to authorize an incoming pairing attempt.
     *
     * This method gets called to request the user to authorize
     * an incoming pairing attempt which would in other circumstances
     * trigger the just-works model.
     *
     * @param device device that invoked the action
     * @param request request to be used for sending reply
     */
    virtual void requestAuthorization(DevicePtr device, const Request<> &request);

    /**
     * Requests the agent to authorize a connection/service request.
     *
     * This method gets called when the Bluetooth daemon
     * needs to authorize a connection/service request.
     *
     * @param device device that invoked the action
     * @param uuid UUID of service
     * @param request request to be used for sending reply
     */
    virtual void authorizeService(DevicePtr device, const QString &uuid, const Request<> &request);

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

#endif // BLUEZQT_AGENT_H
