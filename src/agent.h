/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * SPDX-FileCopyrightText: 2014-2015 David Rosca <nowrep@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef BLUEZQT_AGENT_H
#define BLUEZQT_AGENT_H

#include <QObject>

#include "bluezqt_export.h"
#include "request.h"
#include "types.h"

class QDBusObjectPath;

namespace BluezQt
{
class Device;

/*!
 * \inmodule BluezQt
 * \class BluezQt::Agent
 * \inheaderfile BluezQt/Agent
 * \brief Bluetooth agent.
 *
 * This class represents a Bluetooth agent.
 *
 * The agent is used in pairing process to do various actions.
 *
 * \note The return value of requests will be sent asynchronously with Request class.
 *       It is also possible to cancel/reject all requests.
 */
class BLUEZQT_EXPORT Agent : public QObject
{
    Q_OBJECT

    /*!
     * \property BluezQt::Agent::capability
     */
    Q_PROPERTY(Capability capability READ capability)

public:
    /*!
     * The input/output capabilities of Agent.
     * \value DisplayOnly
     * \value DisplayYesNo
     * \value KeyboardOnly
     * \value NoInputNoOutput
     */
    enum Capability {
        DisplayOnly = 0,
        DisplayYesNo = 1,
        KeyboardOnly = 2,
        NoInputNoOutput = 3,
    };
    Q_ENUM(Capability)

    /*!
     * Creates a new Agent object as a child of \a parent.
     */
    explicit Agent(QObject *parent = nullptr);

    /*!
     * Returns the D-Bus object path where the agent will be registered.
     *
     * \note You must provide valid object path!
     */
    virtual QDBusObjectPath objectPath() const = 0;

    /*!
     * Returns the input/output capability of the agent.
     *
     * By default, this method returns DisplayYesNo.
     */
    virtual Capability capability() const;

    /*!
     * Requests a PIN code from the agent.
     *
     * This method gets called when the Bluetooth daemon
     * needs to get the PIN code for an authentication.
     *
     * The return value should be a string of 1-16 characters
     * length. The string can be alphanumeric.
     *
     * \a device The device that invoked the action.
     *
     * \a request The request to be used for sending reply.
     */
    virtual void requestPinCode(DevicePtr device, const Request<QString> &request);

    /*!
     * Requests the agent to display a PIN code.
     *
     * This method gets called when the Bluetooth daemon
     * needs to display a pincode for an authentication.
     *
     * When the PIN code needs no longer to be displayed,
     * the cancel() method will be called.
     *
     * \a device The device that invoked the action.
     *
     * \a pinCode The PIN code to be displayed.
     */
    virtual void displayPinCode(DevicePtr device, const QString &pinCode);

    /*!
     * Requests a passkey from the agent.
     *
     * This method gets called when the Bluetooth daemon
     * needs to get the passkey for an authentication.
     *
     * The return value should be a numeric value between 0-999999.
     *
     * \a device The device that invoked the action.
     *
     * \a request The request to be used for sending a reply.
     */
    virtual void requestPasskey(DevicePtr device, const Request<quint32> &request);

    /*!
     * Requests the agent to display a passkey.
     *
     * This method gets called when the Bluetooth daemon
     * needs to display a passkey for an authentication.
     *
     * When the passkey needs no longer to be displayed,
     * the cancel() method will be called.
     *
     * \a device The device that invoked the action.
     *
     * \a passkey The passkey to be displayed.
     *
     * \a entered The number of already typed keys on the remote side.
     */
    virtual void displayPasskey(DevicePtr device, const QString &passkey, const QString &entered);

    /*!
     * Requests the agent to confirm a passkey.
     *
     * This method gets called when the Bluetooth daemon
     * needs to confirm a passkey for an authentication.
     *
     * \a device The device that invoked the action.
     *
     * \a passkey The passkey to be confirmed.
     *
     * \a request The request to be used for sending reply.
     */
    virtual void requestConfirmation(DevicePtr device, const QString &passkey, const Request<> &request);

    /*!
     * Requests the agent to authorize an incoming pairing attempt.
     *
     * This method gets called to request the user to authorize
     * an incoming pairing attempt which would in other circumstances
     * trigger the just-works model.
     *
     * \a device The device that invoked the action.
     *
     * \a request The request to be used for sending reply.
     */
    virtual void requestAuthorization(DevicePtr device, const Request<> &request);

    /*!
     * Requests the agent to authorize a connection/service request.
     *
     * This method gets called when the Bluetooth daemon
     * needs to authorize a connection/service request.
     *
     * \a device The device that invoked the action.
     *
     * \a uuid The UUID of the service.
     *
     * \a request The request to be used for sending a reply.
     */
    virtual void authorizeService(DevicePtr device, const QString &uuid, const Request<> &request);

    /*!
     * Indicate that the agent request failed before receiving reply.
     *
     * This method gets called to indicate that the agent
     * request failed before a reply was returned.
     *
     * It cancels the previous request.
     */
    virtual void cancel();

    /*!
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
