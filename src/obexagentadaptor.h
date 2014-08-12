#ifndef QBLUEZ_OBEXAGENTADAPTOR_H
#define QBLUEZ_OBEXAGENTADAPTOR_H

#include <QObject>
#include <QDBusAbstractAdaptor>

#include "request.h"
#include "qbluez_export.h"

class QDBusMessage;
class QDBusObjectPath;

namespace QBluez
{

class ObexTransfer;
class ObexManager;
class ObexAgent;

class ObexAgentAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.bluez.obex.Agent1")

public:
    explicit ObexAgentAdaptor(ObexAgent *parent, ObexManager *manager);

public Q_SLOTS:
    QString AuthorizePush(const QDBusObjectPath &transfer, const QDBusMessage &msg);

    Q_NOREPLY void Cancel();
    Q_NOREPLY void Release();

private Q_SLOTS:
    void transferInitFinished();
    void transferInitError();

private:
    ObexAgent *m_agent;
    ObexManager *m_manager;
    ObexTransfer *m_transfer;
    Request<QString> m_transferRequest;
};

} // namespace QBluez

#endif // QBLUEZ_OBEXAGENTADAPTOR_H
