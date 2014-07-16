#include "obexagent.h"
#include "debug_p.h"

namespace QBluez
{

ObexAgent::ObexAgent(QObject *parent)
    : QObject(parent)
{
}

void ObexAgent::authorizePush(const QDBusObjectPath &transfer, const Request<QString> &request)
{
    Q_UNUSED(transfer)
    Q_UNUSED(request)
}

void ObexAgent::cancel()
{
}

void ObexAgent::release()
{
}

} // namespace QBluez
