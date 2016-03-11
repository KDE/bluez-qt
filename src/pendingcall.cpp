/*
 * BluezQt - Asynchronous Bluez wrapper library
 *
 * Copyright (C) 2014 David Rosca <nowrep@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "pendingcall.h"
#include "obextransfer.h"
#include "obextransfer_p.h"
#include "obexfiletransferentry.h"
#include "bluezqt_dbustypes.h"
#include "debug.h"

#include <QTimer>
#include <QDBusPendingReply>
#include <QDBusPendingCallWatcher>

namespace BluezQt
{

static PendingCall::Error nameToError(const QString &name)
{
    if (name.startsWith(QLatin1String("org.freedesktop.DBus.Error"))) {
        return PendingCall::DBusError;
    }

    if (!name.startsWith(QLatin1String("org.bluez.Error"))) {
        return PendingCall::UnknownError;
    }

#define FROM_BLUEZ_ERROR(string, value) \
    if (errorName == QLatin1String(string)) { \
        return value; \
    }

    const QString &errorName = name.mid(16);
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

class PendingCallPrivate : public QObject
{
public:
    explicit PendingCallPrivate(PendingCall *parent);

    void processReply(QDBusPendingCallWatcher *call);
    void processVoidReply(const QDBusPendingReply<> &reply);
    void processUint32Reply(const QDBusPendingReply<quint32> &reply);
    void processStringReply(const QDBusPendingReply<QString> &reply);
    void processObjectPathReply(const QDBusPendingReply<QDBusObjectPath> &reply);
    void processFileTransferListReply(const QDBusPendingReply<QVariantMapList> &reply);
    void processTransferWithPropertiesReply(const QDBusPendingReply<QDBusObjectPath, QVariantMap> &reply);
    void processError(const QDBusError &m_error);

    void emitFinished();
    void emitDelayedFinished();
    void emitInternalError(const QString &errorText);
    void pendingCallFinished(QDBusPendingCallWatcher *m_watcher);

    PendingCall *q;
    int m_error;
    QString m_errorText;
    QVariant m_userData;
    QVariantList m_value;
    PendingCall::ReturnType m_type;
    QDBusPendingCallWatcher *m_watcher;
};

PendingCallPrivate::PendingCallPrivate(PendingCall *parent)
    : QObject(parent)
    , q(parent)
    , m_error(PendingCall::NoError)
    , m_type(PendingCall::ReturnVoid)
    , m_watcher(0)
{
}

void PendingCallPrivate::processReply(QDBusPendingCallWatcher *call)
{
    switch (m_type) {
    case PendingCall::ReturnVoid:
        processVoidReply(*call);
        break;

    case PendingCall::ReturnUint32:
        processUint32Reply(*call);
        break;

    case PendingCall::ReturnString:
        processStringReply(*call);
        break;

    case PendingCall::ReturnObjectPath:
        processObjectPathReply(*call);
        break;

    case PendingCall::ReturnFileTransferList:
        processFileTransferListReply(*call);
        break;

    case PendingCall::ReturnTransferWithProperties:
        processTransferWithPropertiesReply(*call);
        break;

    default:
        break;
    }
}

void PendingCallPrivate::processVoidReply(const QDBusPendingReply<> &reply)
{
    processError(reply.error());
}

void PendingCallPrivate::processUint32Reply(const QDBusPendingReply<quint32> &reply)
{
    processError(reply.error());
    if (!reply.isError()) {
        m_value.append(reply.value());
    }
}

void PendingCallPrivate::processStringReply(const QDBusPendingReply<QString> &reply)
{
    processError(reply.error());
    if (!reply.isError()) {
        m_value.append(reply.value());
    }
}

void PendingCallPrivate::processObjectPathReply(const QDBusPendingReply<QDBusObjectPath> &reply)
{
    processError(reply.error());
    if (!reply.isError()) {
        m_value.append(QVariant::fromValue(reply.value()));
    }
}

void PendingCallPrivate::processFileTransferListReply(const QDBusPendingReply<QVariantMapList> &reply)
{
    processError(reply.error());
    if (!reply.isError()) {
        QList<ObexFileTransferEntry> items;
        items.reserve(reply.value().size());
        Q_FOREACH (const QVariantMap &map, reply.value()) {
            items.append(ObexFileTransferEntry(map));
        }
        m_value.append(QVariant::fromValue(items));
    }
}

void PendingCallPrivate::processTransferWithPropertiesReply(const QDBusPendingReply<QDBusObjectPath, QVariantMap> &reply)
{
    processError(reply.error());
    if (reply.isError()) {
        return;
    }

    ObexTransferPtr transfer = ObexTransferPtr(new ObexTransfer(reply.argumentAt<0>().path(), reply.argumentAt<1>()));
    transfer->d->q = transfer.toWeakRef();
    transfer->d->m_suspendable = true;
    m_value.append(QVariant::fromValue(transfer));
}

void PendingCallPrivate::processError(const QDBusError &error)
{
    if (error.isValid()) {
        qCWarning(BLUEZQT) << "PendingCall Error:" << error.message();
        m_error = nameToError(error.name());
        m_errorText = error.message();
    }
}

void PendingCallPrivate::emitFinished()
{
    m_watcher->deleteLater();
    m_watcher = Q_NULLPTR;
    Q_EMIT q->finished(q);
    q->deleteLater();
}

void PendingCallPrivate::emitDelayedFinished()
{
    Q_ASSERT(qobject_cast<QTimer*>(sender()));

    Q_EMIT q->finished(q);
    static_cast<QTimer*>(sender())->deleteLater();
}

void PendingCallPrivate::emitInternalError(const QString &errorText)
{
    qCWarning(BLUEZQT) << "PendingCall Internal error:" << errorText;
    m_error = PendingCall::InternalError;
    m_errorText = errorText;
    emitFinished();
}

void PendingCallPrivate::pendingCallFinished(QDBusPendingCallWatcher *watcher)
{
    processReply(watcher);
    emitFinished();
}

PendingCall::PendingCall(const QDBusPendingCall &call, ReturnType type, QObject *parent)
    : QObject(parent)
    , d(new PendingCallPrivate(this))
{
    qDBusRegisterMetaType<QVariantMapList>();

    d->m_type = type;
    d->m_watcher = new QDBusPendingCallWatcher(call, this);

    connect(d->m_watcher, &QDBusPendingCallWatcher::finished, d, &PendingCallPrivate::pendingCallFinished);
}

PendingCall::PendingCall(PendingCall::Error error, const QString &errorText, QObject *parent)
    : QObject(parent)
    , d(new PendingCallPrivate(this))
{
    d->m_error = error;
    d->m_errorText = errorText;

    QTimer *timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->start(0);
    connect(timer, &QTimer::timeout, d, &PendingCallPrivate::emitDelayedFinished);
}

PendingCall::~PendingCall()
{
    delete d;
}

QVariant PendingCall::value() const
{
    if (d->m_value.isEmpty()) {
        return QVariant();
    }
    return d->m_value.first();
}

QVariantList PendingCall::values() const
{
    return d->m_value;
}

int PendingCall::error() const
{
    return d->m_error;
}

QString PendingCall::errorText() const
{
    return d->m_errorText;
}

bool PendingCall::isFinished() const
{
    if (d->m_watcher) {
        return d->m_watcher->isFinished();
    }
    return true;
}

void PendingCall::waitForFinished()
{
    if (d->m_watcher) {
        d->m_watcher->waitForFinished();
    }
}

QVariant PendingCall::userData() const
{
    return d->m_userData;
}

void PendingCall::setUserData(const QVariant &userData)
{
    d->m_userData = userData;
}

} // namespace BluezQt
