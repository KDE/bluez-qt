#ifndef QBLUEZ_OBEXAGENT_H
#define QBLUEZ_OBEXAGENT_H

#include <QObject>

#include "request.h"
#include "qbluez_export.h"

class QDBusObjectPath;

namespace QBluez
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
class QBLUEZ_EXPORT ObexAgent : public QObject
{
    Q_OBJECT

public:
    /**
     * Creates a new ObexAgent object.
     *
     * @param parent
     */
    explicit ObexAgent(QObject *parent = 0);

    /**
     * D-Bus object path of the agent.
     *
     * The path where the agent will be registered.
     *
     * You must provide valid object path!
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
     * The @p transfer object will contain a fileName property that
     * contains the default location and name that can be returned.
     *
     * @param transfer transfer object
     * @param request request to be used for sending reply
     */
    virtual void authorizePush(ObexTransfer *transfer, const Request<QString> &request);

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

} // namespace QBluez

#endif // QBLUEZ_OBEXAGENT_H
