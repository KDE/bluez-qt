#ifndef QBLUEZ_AGENT_H
#define QBLUEZ_AGENT_H

#include <QObject>

#include "request.h"
#include "qbluez_export.h"

class QDBusObjectPath;

namespace QBluez
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
class QBLUEZ_EXPORT Agent : public QObject
{
    Q_OBJECT

public:
    /**
     * Constructs a new Agent object.
     *
     * @param parent
     */
    explicit Agent(QObject *parent = 0);

    /**
     * D-Bus object path of the agent.
     *
     * The path where the agent will be registered.
     * You must provide valid object path!
     *
     * @return object path of agent
     */
    virtual QDBusObjectPath objectPath() const = 0;

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
    virtual void requestPinCode(Device *device, const Request<QString> &request);

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
    virtual void displayPinCode(Device *device, const QString &pinCode);

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
    virtual void requestPasskey(Device *device, const Request<quint32> &request);

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
    virtual void displayPasskey(Device *device, const QString &passkey, const QString &entered);

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
    virtual void requestConfirmation(Device *device, const QString &passkey, const Request<> &request);

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
    virtual void requestAuthorization(Device *device, const Request<> &request);

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
    virtual void authorizeService(Device *device, const QString &uuid, const Request<> &request);

    /**
     * Indicate that the agent request failed before receiving reply.
     *
     * This method gets called to indicate that the agent
     * request failed before a reply was returned.
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

} // namespace QBluez

#endif // QBLUEZ_AGENT_H
