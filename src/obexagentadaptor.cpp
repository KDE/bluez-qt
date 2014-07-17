#include "obexagentadaptor.h"
#include "obexagent.h"
#include "obexmanager.h"
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

void ObexAgentAdaptor::AuthorizePush(const QDBusObjectPath &transfer, const QDBusMessage &msg)
{
    msg.setDelayedReply(true);
    m_agent->authorizePush(transfer, Request<QString>(OrgBluezObexAgent, msg));
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
