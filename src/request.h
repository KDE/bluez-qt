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

template<typename T = void>
class Request
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

    void accept(T returnValue) const
    {
        qbluez_acceptRequest(m_type, returnValue, m_message);
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
