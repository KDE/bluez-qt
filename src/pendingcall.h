#ifndef QBLUEZ_PENDINGCALL_H
#define QBLUEZ_PENDINGCALL_H

#include <QObject>
#include <QDBusPendingReply>

#include "qbluez_export.h"

namespace QBluez
{

class QBLUEZ_EXPORT PendingCall : public QObject
{
    Q_OBJECT

public:
    enum Error {
        NoError = 0,
        NotReady = 1,
        Failed = 2,
        Rejected = 3,
        Canceled = 4,
        InvalidArguments = 5,
        AlreadyExists = 6,
        DoesNotExist = 7,
        InProgress = 8,
        AlreadyConnected = 9,
        ConnectFailed = 10,
        NotConnected = 11,
        NotSupported = 12,
        NotAuthorized = 13,
        AuthenticationCanceled = 14,
        AuthenticationFailed = 15,
        AuthenticationRejected = 16,
        AuthenticationTimeout = 17,
        ConnectionAttemptFailed = 18,
        UnknownError = 100
    };

    PendingCall(const QDBusPendingReply<> &reply, QObject *parent);
    ~PendingCall();

    int error() const;
    QString errorText() const;

Q_SIGNALS:
    void finished(PendingCall *call);

private:
    class PendingCallPrivate *d;
    friend class PendingCallPrivate;
};

} // namespace QBluez

#endif // QBLUEZ_PENDINGCALL_H
