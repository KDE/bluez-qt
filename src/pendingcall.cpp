#include "pendingcall.h"
#include "obexsession.h"
#include "obexsession_p.h"
#include "debug_p.h"

#include <QDBusPendingCallWatcher>

namespace QBluez
{

static PendingCall::Error nameToError(const QString &name)
{
    if (!name.startsWith(QLatin1String("org.bluez.Error"))) {
        return PendingCall::UnknownError;
    }

#define FROM_BLUEZ_ERROR(string, value) \
    if (errorName == QLatin1String(string)) { \
        return value; \
    }

    const QString &errorName = name.mid(15);
    FROM_BLUEZ_ERROR("NotReady", PendingCall::NotReady);
    FROM_BLUEZ_ERROR("Failed", PendingCall::Failed);
    FROM_BLUEZ_ERROR("Rejected", PendingCall::Rejected);
    FROM_BLUEZ_ERROR("Canceled", PendingCall::Canceled);
    FROM_BLUEZ_ERROR("InvalidArguments", PendingCall::InvalidArguments);
    FROM_BLUEZ_ERROR("AlreadyExists", PendingCall::AlreadyExists);
    FROM_BLUEZ_ERROR("DoesNotExist", PendingCall::DoesNotExist);
    FROM_BLUEZ_ERROR("AlreadyConnected", PendingCall::AlreadyConnected);
    FROM_BLUEZ_ERROR("ConnectFailed", PendingCall::ConnectFailed);
    FROM_BLUEZ_ERROR("NotConnected", PendingCall::NotConnected);
    FROM_BLUEZ_ERROR("NotSupported", PendingCall::NotSupported);
    FROM_BLUEZ_ERROR("NotAuthorized", PendingCall::NotAuthorized);
    FROM_BLUEZ_ERROR("AuthenticationCanceled", PendingCall::AuthenticationCanceled);
    FROM_BLUEZ_ERROR("AuthenticationFailed", PendingCall::AuthenticationFailed);
    FROM_BLUEZ_ERROR("AuthenticationRejected", PendingCall::AuthenticationRejected);
    FROM_BLUEZ_ERROR("AuthenticationTimeout", PendingCall::AuthenticationTimeout);
    FROM_BLUEZ_ERROR("ConnectionAttemptFailed", PendingCall::ConnectionAttemptFailed);
#undef FROM_BLUEZ_ERROR

    return PendingCall::UnknownError;
}

class PendingCallPrivate
{
public:
    int error;
    QString errorText;
    QVariantList value;
    PendingCall::ReturnType type;
    QDBusPendingCallWatcher *watcher;
};

PendingCall::PendingCall(const QDBusPendingCall &call, ReturnType type, QObject *parent)
    : QObject(parent)
    , d(new PendingCallPrivate)
{
    d->error = NoError;
    d->type = type;
    d->watcher = new QDBusPendingCallWatcher(call, this);

    connect(d->watcher, &QDBusPendingCallWatcher::finished, [ this ]() {
        processReply(d->watcher);
        d->watcher->deleteLater();
        d->watcher = Q_NULLPTR;
        if (d->type == ReturnObexSession) {
            loadObexSession();
        } else {
            Q_EMIT finished(this);
            deleteLater();
        }
    });
}

PendingCall::~PendingCall()
{
    delete d;
}

QVariantList PendingCall::value() const
{
    return d->value;
}

int PendingCall::error() const
{
    return d->error;
}

QString PendingCall::errorText() const
{
    return d->errorText;
}

bool PendingCall::isFinished() const
{
    if (d->watcher) {
        return d->watcher->isFinished();
    }
    return true;
}

void PendingCall::waitForFinished()
{
    if (d->watcher) {
        d->watcher->waitForFinished();
    }
}

void PendingCall::processReply(QDBusPendingCallWatcher *call)
{
    switch (d->type) {
    case ReturnVoid: {
        const QDBusPendingReply<> &reply = *call;
        processError(reply.error());
        break;
    }

    case ReturnString: {
        const QDBusPendingReply<QString> &reply = *call;
        processError(reply.error());
        if (!reply.isError()) {
            d->value.append(reply.argumentAt(0));
        }
        break;
    }

    case ReturnObjectPath:
    case ReturnObexSession: {
        const QDBusPendingReply<QDBusObjectPath> &reply = *call;
        processError(reply.error());
        if (!reply.isError()) {
            d->value.append(reply.argumentAt(0));
        }
        break;
    }

    default:
        break;
    }
}

void PendingCall::processError(const QDBusError &error)
{
    if (error.isValid()) {
        qCWarning(QBLUEZ) << "PendingCall Error:" << error.message();
        d->error = nameToError(error.name());
        d->errorText = error.message();
    }
}

void PendingCall::loadObexSession()
{
    ObexSession *session = new ObexSession(d->value.at(0).value<QDBusObjectPath>().path(), parent());
    d->value.clear();

    session->d->init();
    connect(session->d, &ObexSessionPrivate::initFinished, [ this, session ]() {
        d->value.append(QVariant::fromValue(session));
        Q_EMIT finished(this);
        deleteLater();
    });
    connect(session->d, &ObexSessionPrivate::initError, [ this ](const QString &errorText) {
        d->error = InternalError;
        d->errorText = errorText;
        Q_EMIT finished(this);
        deleteLater();
    });
}

} // namespace QBluez
