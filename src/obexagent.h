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

class QBLUEZ_EXPORT ObexAgent : public QObject
{
    Q_OBJECT

public:
    explicit ObexAgent(QObject *parent = 0);

    virtual QDBusObjectPath objectPath() const = 0;

public Q_SLOTS:
    virtual void authorizePush(ObexTransfer *transfer, const Request<QString> &request);

    virtual void cancel();
    virtual void release();
};

} // namespace QBluez

#endif // QBLUEZ_OBEXAGENT_H
