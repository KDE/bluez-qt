#ifndef QBLUEZ_OBEXAGENTADAPTOR_H
#define QBLUEZ_OBEXAGENTADAPTOR_H

#include <QObject>
#include <QDBusAbstractAdaptor>

#include "qbluez_export.h"

class QDBusMessage;
class QDBusObjectPath;

namespace QBluez
{

class ObexManager;
class ObexAgent;

class ObexAgentAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.bluez.obex.Agent1")

public:
    explicit ObexAgentAdaptor(ObexAgent *parent, ObexManager *manager);
    ~ObexAgentAdaptor();

public Q_SLOTS:
    void AuthorizePush(const QDBusObjectPath &transfer, const QDBusMessage &msg);

    Q_NOREPLY void Cancel();
    Q_NOREPLY void Release();

private:
    ObexAgent *m_agent;
    ObexManager *m_manager;
};

} // namespace QBluez

#endif // QBLUEZ_OBEXAGENTADAPTOR_H
