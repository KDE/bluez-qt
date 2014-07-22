#include "obexobjectpush.h"
#include "pendingcall.h"

#include "obexobjectpush1.h"

namespace QBluez
{

typedef org::bluez::obex::ObjectPush1 BluezObjectPush;

class ObexObjectPushPrivate
{
public:
    ObexObjectPush *q;
    BluezObjectPush *m_bluezObjectPush;
};

ObexObjectPush::ObexObjectPush(const QDBusObjectPath &path, QObject *parent)
    : QObject(parent)
    , d(new ObexObjectPushPrivate)
{
    d->m_bluezObjectPush = new BluezObjectPush(QStringLiteral("org.bluez.obex"),
                                               path.path(), QDBusConnection::sessionBus(), this);
}

ObexObjectPush::~ObexObjectPush()
{
    delete d;
}

QDBusObjectPath ObexObjectPush::objectPath() const
{
    return QDBusObjectPath(d->m_bluezObjectPush->path());
}

PendingCall *ObexObjectPush::sendFile(const QString &filename)
{
    return new PendingCall(d->m_bluezObjectPush->SendFile(filename),
                           PendingCall::ReturnTransferWithProperties, this);
}

PendingCall *ObexObjectPush::pullBusinessCard(const QString &targetFilename)
{
    return new PendingCall(d->m_bluezObjectPush->PullBusinessCard(targetFilename),
                           PendingCall::ReturnTransferWithProperties, this);
}

PendingCall *ObexObjectPush::exchangeBusinessCards(const QString &clientFilename, const QString &targetFilename)
{
    return new PendingCall(d->m_bluezObjectPush->ExchangeBusinessCards(clientFilename, targetFilename),
                           PendingCall::ReturnTransferWithProperties, this);
}

} // namespace QBluez
