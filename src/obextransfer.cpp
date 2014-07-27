#include "obextransfer.h"
#include "obextransfer_p.h"
#include "obexsession.h"
#include "obexsession_p.h"
#include "pendingcall.h"

#include "obextransfer1.h"
#include "dbusproperties.h"

namespace QBluez
{

static ObexTransfer::Status stringToStatus(const QString &status)
{
    if (status == QLatin1String("queued")) {
        return ObexTransfer::Queued;
    } else if (status == QLatin1String("active")) {
        return ObexTransfer::Active;
    } else if (status == QLatin1String("suspended")) {
        return ObexTransfer::Suspended;
    } else if (status == QLatin1String("complete")) {
        return ObexTransfer::Complete;
    } else if (status == QLatin1String("error")) {
        return ObexTransfer::Error;
    }
    return ObexTransfer::Unknown;
}

ObexTransferPrivate::ObexTransferPrivate(ObexTransfer *q, const QString &path)
    : QObject(q)
    , q(q)
    , m_dbusProperties(0)
{
    m_bluezTransfer = new BluezTransfer(QStringLiteral("org.bluez.obex"),
                                        path, QDBusConnection::sessionBus(), this);
}

void ObexTransferPrivate::init()
{
    m_dbusProperties = new DBusProperties(QStringLiteral("org.bluez.obex"), m_bluezTransfer->path(),
                                          QDBusConnection::sessionBus(), this);

    connect(m_dbusProperties, &DBusProperties::PropertiesChanged,
            this, &ObexTransferPrivate::propertiesChanged, Qt::QueuedConnection);

    const QDBusPendingReply<QVariantMap> &call = m_dbusProperties->GetAll(QStringLiteral("org.bluez.obex.Transfer1"));
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);

    connect(watcher, &QDBusPendingCallWatcher::finished, [ this, watcher ]() {
        const QDBusPendingReply<QVariantMap> &reply = *watcher;
        watcher->deleteLater();

        if (reply.isError()) {
            Q_EMIT initError(reply.error().message());
            return;
        }

        const QVariantMap &properties = reply.value();

        m_status = stringToStatus(properties.value(QStringLiteral("Status")).toString());
        m_session = new ObexSession(properties.value(QStringLiteral("Session")).value<QDBusObjectPath>().path());
        m_name = properties.value(QStringLiteral("Name")).toString();
        m_type = properties.value(QStringLiteral("Type")).toString();
        m_time = properties.value(QStringLiteral("Time")).toUInt();
        m_size = properties.value(QStringLiteral("Size")).toUInt();
        m_transferred = properties.value(QStringLiteral("Transferred")).toUInt();
        m_fileName = properties.value(QStringLiteral("Filename")).toString();

        m_session->d->init();
        connect(m_session->d, &ObexSessionPrivate::initFinished, [ this ]() {
            Q_EMIT initFinished();
        });
        connect(m_session->d, &ObexSessionPrivate::initError, [ this ](const QString &errorText) {
            Q_EMIT initError(errorText);
        });
    });
}

void ObexTransferPrivate::propertiesChanged(const QString &interface, const QVariantMap &changed, const QStringList &invalidated)
{
    Q_UNUSED(interface)
    Q_UNUSED(invalidated)

    QVariantMap::const_iterator i;
    for (i = changed.constBegin(); i != changed.constEnd(); ++i) {
        const QVariant &value = i.value();
        const QString &property = i.key();

        if (property == QLatin1String("Status")) {
            ObexTransfer::Status changedStatus = stringToStatus(value.toString());
            if (m_status != changedStatus) {
                m_status = changedStatus;
                Q_EMIT q->statusChanged(m_status);
            }
        } else if (property == QLatin1String("Transferred")) {
            if (m_transferred != value.toUInt()) {
                m_transferred = value.toUInt();
                Q_EMIT q->transferredChanged(m_transferred);
            }
        } else if (property == QLatin1String("Filename")) {
            if (m_fileName != value.toString()) {
                m_fileName = value.toString();
                Q_EMIT q->fileNameChanged(m_fileName);
            }
        }
    }
}

ObexTransfer::ObexTransfer(const QString &path, QObject *parent)
    : QObject(parent)
    , d(new ObexTransferPrivate(this, path))
{
}

ObexTransfer::~ObexTransfer()
{
    delete d;
}

QDBusObjectPath ObexTransfer::objectPath() const
{
    return QDBusObjectPath(d->m_bluezTransfer->path());
}

ObexTransfer::Status ObexTransfer::status() const
{
    return d->m_status;
}

ObexSession *ObexTransfer::session() const
{
    return d->m_session;
}

QString ObexTransfer::name() const
{
    return d->m_name;
}

QString ObexTransfer::type() const
{
    return d->m_type;
}

quint64 ObexTransfer::time() const
{
    return d->m_time;
}

quint64 ObexTransfer::size() const
{
    return d->m_size;
}

quint64 ObexTransfer::transferred() const
{
    return d->m_transferred;
}

QString ObexTransfer::fileName() const
{
    return d->m_fileName;
}

bool ObexTransfer::isSuspendable() const
{
    return false;
}

PendingCall *ObexTransfer::cancel()
{
    return new PendingCall(d->m_bluezTransfer->Cancel(), PendingCall::ReturnVoid, this);
}

PendingCall *ObexTransfer::suspend()
{
    return new PendingCall(d->m_bluezTransfer->Suspend(), PendingCall::ReturnVoid, this);
}

PendingCall *ObexTransfer::resume()
{
    return new PendingCall(d->m_bluezTransfer->Resume(), PendingCall::ReturnVoid, this);
}

} // namespace QBluez
