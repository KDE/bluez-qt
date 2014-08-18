#include "obexsession.h"
#include "obexsession_p.h"
#include "pendingcall.h"
#include "utils_p.h"

namespace QBluez
{

ObexSessionPrivate::ObexSessionPrivate(ObexSession *q, const QString &path)
    : QObject(q)
    , q(q)
{
    m_bluezSession = new BluezSession(Strings::orgBluezObex(),
                                      path, DBusConnection::orgBluezObex(), this);
}

void ObexSessionPrivate::init()
{
    DBusProperties dbusProperties(Strings::orgBluezObex(), m_bluezSession->path(),
                                  DBusConnection::orgBluezObex(), this);

    const QDBusPendingReply<QVariantMap> &call = dbusProperties.GetAll(Strings::orgBluezObexSession1());
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, &ObexSessionPrivate::getPropertiesFinished);
}

void ObexSessionPrivate::getPropertiesFinished(QDBusPendingCallWatcher *watcher)
{
    const QDBusPendingReply<QVariantMap> &reply = *watcher;
    watcher->deleteLater();

    if (reply.isError()) {
        Q_EMIT initError(reply.error().message());
        return;
    }

    const QVariantMap &properties = reply.value();

    m_source = properties.value(QStringLiteral("Source")).toString();
    m_destination = properties.value(QStringLiteral("Destination")).toString();
    m_channel = properties.value(QStringLiteral("Channel")).toUInt();
    m_target = properties.value(QStringLiteral("Target")).toString();
    m_root = properties.value(QStringLiteral("Root")).toString();

    Q_EMIT initFinished();
}

ObexSession::ObexSession(const QString &path, QObject *parent)
    : QObject(parent)
    , d(new ObexSessionPrivate(this, path))
{
}

ObexSession::~ObexSession()
{
    delete d;
}

QString ObexSession::source() const
{
    return d->m_source;
}

QString ObexSession::destination() const
{
    return d->m_destination;
}

quint8 ObexSession::channel() const
{
    return d->m_channel;
}

QString ObexSession::target() const
{
    return d->m_target;
}

QString ObexSession::root() const
{
    return d->m_root;
}

PendingCall *ObexSession::getCapabilities()
{
    return new PendingCall(d->m_bluezSession->GetCapabilities(), PendingCall::ReturnString, this);
}

} // namespace QBluez

#include "obexsession.moc"
