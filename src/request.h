#ifndef QBLUEZ_REQUEST_H
#define QBLUEZ_REQUEST_H

#include <QDBusMessage>

#include "qbluez_export.h"

namespace QBluez
{

QBLUEZ_EXPORT void qbluez_acceptRequest(const QDBusMessage &req, const QVariant &val);
QBLUEZ_EXPORT void qbluez_rejectRequest(const QDBusMessage &req, const QString &iface);
QBLUEZ_EXPORT void qbluez_cancelRequest(const QDBusMessage &req, const QString &iface);

template<typename T>
class Request
{
public:
    Request()
    {
    }

    Request(const QString &iface, const QDBusMessage &message)
        : m_iface(iface)
        , m_message(message)
    {
    }

    void accept(T returnValue) const
    {
        qbluez_acceptRequest(m_message, returnValue);
    }

    void reject() const
    {
        qbluez_rejectRequest(m_message, m_iface);
    }

    void cancel() const
    {
        qbluez_cancelRequest(m_message, m_iface);
    }

private:
    QString m_iface;
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

    Request(const QString &iface, const QDBusMessage &message)
        : m_iface(iface)
        , m_message(message)
    {
    }

    void accept() const
    {
        qbluez_acceptRequest(m_message, QVariant());
    }

    void reject() const
    {
        qbluez_rejectRequest(m_message, m_iface);
    }

    void cancel() const
    {
        qbluez_cancelRequest(m_message, m_iface);
    }

private:
    QString m_iface;
    QDBusMessage m_message;
};

} // namespace QBluez

#endif // QBLUEZ_REQUEST_H
