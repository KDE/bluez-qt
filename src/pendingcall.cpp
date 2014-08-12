#include "pendingcall.h"
#include "obextransfer.h"
#include "obextransfer_p.h"
#include "debug_p.h"

#include <QTimer>
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

static QDateTime dateTimeFromTransfer(const QString &value)
{
    return QDateTime::fromString(value, QStringLiteral("yyyyMMddThhmmssZ"));
}

static QList<ObexFileTransfer::Item> toFileTransferList(const QVariantMapList &list)
{
    QList<ObexFileTransfer::Item> items;

    Q_FOREACH (const QVariantMap &map, list) {
        ObexFileTransfer::Item item;
        if (map.value(QStringLiteral("Type")).toString() == QLatin1String("folder")) {
            item.type = ObexFileTransfer::Item::Folder;
        } else {
            item.type = ObexFileTransfer::Item::File;
        }
        item.name = map.value(QStringLiteral("Name")).toString();
        item.label = map.value(QStringLiteral("Label")).toString();
        item.size = map.value(QStringLiteral("Size")).toUInt();
        item.permissions = map.value(QStringLiteral("User-perm")).toString();
        item.memoryType = map.value(QStringLiteral("Mem-type")).toString();
        item.modified = dateTimeFromTransfer(map.value(QStringLiteral("Modified")).toString());
        items.append(item);
    }

    return items;
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
    qDBusRegisterMetaType<QVariantMapList>();

    d->error = NoError;
    d->type = type;
    d->watcher = new QDBusPendingCallWatcher(call, this);

    connect(d->watcher, &QDBusPendingCallWatcher::finished, this, &PendingCall::pendingCallFinished);
}

PendingCall::PendingCall(PendingCall::Error error, const QString &errorText, QObject *parent)
    : QObject(parent)
    , d(new PendingCallPrivate)
{
    d->error = error;
    d->errorText = errorText;
    d->watcher = Q_NULLPTR;

    QTimer *timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->start(0);
    connect(timer, &QTimer::timeout, this, &PendingCall::emitDelayedFinished);
}

PendingCall::~PendingCall()
{
    delete d;
}

QVariant PendingCall::value() const
{
    if (d->value.isEmpty()) {
        return QVariant();
    }
    return d->value.first();
}

QVariantList PendingCall::values() const
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

bool PendingCall::processReply(QDBusPendingCallWatcher *call)
{
    switch (d->type) {
    case ReturnVoid:
        return processVoidReply(*call);

    case ReturnString:
        return processStringReply(*call);

    case ReturnObjectPath:
        return processObjectPathReply(*call);

    case ReturnFileTransferList:
        return processFileTransferListReply(*call);

    case ReturnTransferWithProperties:
        return processTransferWithPropertiesReply(*call);

    default:
        break;
    }

    return true;
}

bool PendingCall::processVoidReply(const QDBusPendingReply<> &reply)
{
    processError(reply.error());
    return true;
}

bool PendingCall::processStringReply(const QDBusPendingReply<QString> &reply)
{
    processError(reply.error());
    if (!reply.isError()) {
        d->value.append(reply.argumentAt(0));
    }
    return true;
}

bool PendingCall::processObjectPathReply(const QDBusPendingReply<QDBusObjectPath> &reply)
{
    processError(reply.error());
    if (!reply.isError()) {
        d->value.append(reply.argumentAt(0));
    }
    return true;
}

bool PendingCall::processFileTransferListReply(const QDBusPendingReply<QVariantMapList> &reply)
{
    processError(reply.error());
    if (!reply.isError()) {
        d->value.append(QVariant::fromValue(toFileTransferList(reply.value())));
    }
    return true;
}

bool PendingCall::processTransferWithPropertiesReply(const QDBusPendingReply<QDBusObjectPath, QVariantMap> &reply)
{
    processError(reply.error());
    if (reply.isError()) {
        return true;
    }
    ObexTransfer *transfer = new ObexTransfer(reply.argumentAt(0).value<QDBusObjectPath>().path(), 0);
    d->value.append(QVariant::fromValue(transfer));

    transfer->d->init();
    connect(transfer->d, &ObexTransferPrivate::initFinished, this, &PendingCall::emitFinished);
    connect(transfer->d, &ObexTransferPrivate::initError, this, &PendingCall::emitInternalError);
    return false;
}

void PendingCall::processError(const QDBusError &error)
{
    if (error.isValid()) {
        qCWarning(QBLUEZ) << "PendingCall Error:" << error.message();
        d->error = nameToError(error.name());
        d->errorText = error.message();
    }
}

void PendingCall::emitFinished()
{
    d->watcher->deleteLater();
    d->watcher = Q_NULLPTR;
    Q_EMIT finished(this);
    deleteLater();
}

void PendingCall::emitDelayedFinished()
{
    Q_ASSERT(qobject_cast<QTimer*>(sender()));

    Q_EMIT finished(this);
    static_cast<QTimer*>(sender())->deleteLater();
}

void PendingCall::emitInternalError(const QString &errorText)
{
    qCWarning(QBLUEZ) << "PendingCall Error:" << errorText;
    d->error = InternalError;
    d->errorText = errorText;
    emitFinished();
}

void PendingCall::pendingCallFinished(QDBusPendingCallWatcher *watcher)
{
    if (processReply(watcher)) {
        emitFinished();
    }
}

} // namespace QBluez
