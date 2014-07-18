#include "obexagentadaptor.h"
#include "obexagent.h"
#include "obexmanager.h"
#include "obextransfer.h"
#include "obextransfer_p.h"
#include "request.h"

#include <QDBusObjectPath>

namespace QBluez
{

ObexAgentAdaptor::ObexAgentAdaptor(ObexAgent *parent, ObexManager *manager)
    : QDBusAbstractAdaptor(parent)
    , m_agent(parent)
    , m_manager(manager)
{
}

QString ObexAgentAdaptor::AuthorizePush(const QDBusObjectPath &transfer, const QDBusMessage &msg)
{
    msg.setDelayedReply(true);
    Request<QString> req(OrgBluezObexAgent, msg);

    ObexTransfer *t = new ObexTransfer(transfer.path(), this);
    t->d->init();
    connect(t->d, &ObexTransferPrivate::initFinished, [ this, t, req ]() {
        m_agent->authorizePush(t, req);
    });
    connect(t->d, &ObexTransferPrivate::initError, [ this, t, req ]() {
        t->deleteLater();
        req.cancel();
    });

    return QString();
}

void ObexAgentAdaptor::Cancel()
{
    m_agent->cancel();
}

void ObexAgentAdaptor::Release()
{
    m_agent->release();
}

} // namespace QBluez
