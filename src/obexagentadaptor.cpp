#include "obexagentadaptor.h"
#include "obexagent.h"
#include "obexmanager.h"
#include "obextransfer.h"
#include "obextransfer_p.h"

#include <QDBusObjectPath>

namespace QBluez
{

ObexAgentAdaptor::ObexAgentAdaptor(ObexAgent *parent, ObexManager *manager)
    : QDBusAbstractAdaptor(parent)
    , m_agent(parent)
    , m_manager(manager)
    , m_transfer(0)
{
}

QString ObexAgentAdaptor::AuthorizePush(const QDBusObjectPath &transfer, const QDBusMessage &msg)
{
    msg.setDelayedReply(true);
    m_transferRequest = Request<QString>(OrgBluezObexAgent, msg);

    m_transfer = new ObexTransfer(transfer.path(), this);
    m_transfer->d->init();
    connect(m_transfer->d, &ObexTransferPrivate::initFinished, this, &ObexAgentAdaptor::transferInitFinished);
    connect(m_transfer->d, &ObexTransferPrivate::initError, this, &ObexAgentAdaptor::transferInitError);

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

void ObexAgentAdaptor::transferInitFinished()
{
    m_agent->authorizePush(m_transfer, m_transferRequest);
}

void ObexAgentAdaptor::transferInitError()
{
    m_transfer->deleteLater();
    m_transferRequest.cancel();
}

} // namespace QBluez
