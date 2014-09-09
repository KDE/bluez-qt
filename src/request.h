#ifndef QBLUEZ_REQUEST_H
#define QBLUEZ_REQUEST_H

#include <QDBusMessage>

#include "qbluez_export.h"

namespace QBluez
{

enum AgentType {
    OrgBluezAgent,
    OrgBluezObexAgent
};

QBLUEZ_EXPORT void qbluez_acceptRequest(AgentType type, const QVariant &val, const QDBusMessage &req);
QBLUEZ_EXPORT void qbluez_rejectRequest(AgentType type, const QDBusMessage &req);
QBLUEZ_EXPORT void qbluez_cancelRequest(AgentType type, const QDBusMessage &req);

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
     * Constructs a new Request object.
     */
    Request()
    {
    }

    /**
     * Constructs a new Request object.
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
        qbluez_acceptRequest(m_type, returnValue, m_message);
    }

    /**
     * Rejects the request.
     *
     * This method should be called to send an error reply to
     * indicate the request was rejected.
     */
    void reject() const
    {
        qbluez_rejectRequest(m_type, m_message);
    }

    /**
     * Cancels the request.
     *
     * This method should be called to send an error reply to
     * indicate the request was canceled.
     */
    void cancel() const
    {
        qbluez_cancelRequest(m_type, m_message);
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
        qbluez_acceptRequest(m_type, QVariant(), m_message);
    }

    void reject() const
    {
        qbluez_rejectRequest(m_type, m_message);
    }

    void cancel() const
    {
        qbluez_cancelRequest(m_type, m_message);
    }

private:
    AgentType m_type;
    QDBusMessage m_message;
};

} // namespace QBluez

#endif // QBLUEZ_REQUEST_H
