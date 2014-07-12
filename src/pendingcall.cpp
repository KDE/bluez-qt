#include "pendingcall.h"
#include "debug_p.h"

#include <QDBusPendingCallWatcher>

namespace QBluez
{

static PendingCall::Error nameToError(const QString &name)
{
    if (!name.startsWith(QLatin1String("org.bluez.Error"))) {
        return PendingCall::UnknownError;
    }

#define QBLUEZ_ERROR(string, value) \
    if (errorName == QLatin1String(string)) { \
        return value; \
    }

    const QString &errorName = name.mid(15);
    QBLUEZ_ERROR("NotReady", PendingCall::NotReady);
    QBLUEZ_ERROR("Failed", PendingCall::Failed);
    QBLUEZ_ERROR("Rejected", PendingCall::Rejected);
    QBLUEZ_ERROR("Canceled", PendingCall::Canceled);
    QBLUEZ_ERROR("InvalidArguments", PendingCall::InvalidArguments);
    QBLUEZ_ERROR("AlreadyExists", PendingCall::AlreadyExists);
    QBLUEZ_ERROR("DoesNotExist", PendingCall::DoesNotExist);
    QBLUEZ_ERROR("AlreadyConnected", PendingCall::AlreadyConnected);
    QBLUEZ_ERROR("ConnectFailed", PendingCall::ConnectFailed);
    QBLUEZ_ERROR("NotConnected", PendingCall::NotConnected);
    QBLUEZ_ERROR("NotSupported", PendingCall::NotSupported);
    QBLUEZ_ERROR("NotAuthorized", PendingCall::NotAuthorized);
    QBLUEZ_ERROR("AuthenticationCanceled", PendingCall::AuthenticationCanceled);
    QBLUEZ_ERROR("AuthenticationFailed", PendingCall::AuthenticationFailed);
    QBLUEZ_ERROR("AuthenticationRejected", PendingCall::AuthenticationRejected);
    QBLUEZ_ERROR("AuthenticationTimeout", PendingCall::AuthenticationTimeout);
    QBLUEZ_ERROR("ConnectionAttemptFailed", PendingCall::ConnectionAttemptFailed);
#undef QBLUEZ_ERROR

    return PendingCall::UnknownError;
}

class PendingCallPrivate
{
public:
    int error;
    QString errorText;
    QDBusPendingReply<> reply;
    QDBusPendingCallWatcher *watcher;
};

PendingCall::PendingCall(const QDBusPendingReply<> &reply, QObject *parent)
    : QObject(parent)
    , d(new PendingCallPrivate)
{
    d->error = NoError;
    d->reply = reply;
    d->watcher = new QDBusPendingCallWatcher(reply, this);

    connect(d->watcher, &QDBusPendingCallWatcher::finished, [ this ]() {
        const QDBusPendingReply<> &reply = *d->watcher;
        d->watcher->deleteLater();
        d->watcher = 0;

        if (reply.isError()) {
            qCWarning(QBLUEZ) << "PendingCall Error:" << reply.error().message();

            d->error = nameToError(reply.error().name());
            d->errorText = reply.error().message();
        }

        Q_EMIT finished(this);
        deleteLater();
    });
}

PendingCall::~PendingCall()
{
    delete d;
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

} // namespace QBluez
