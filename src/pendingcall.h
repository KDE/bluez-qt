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

    Q_ENUMS(Error)
    Q_PROPERTY(QVariant value READ value)
    Q_PROPERTY(QVariantList values READ values)
    Q_PROPERTY(int error READ error)
    Q_PROPERTY(QString errorText READ errorText)
    Q_PROPERTY(bool finished READ isFinished)

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
        InternalError = 99,
        UnknownError = 100
    };

    ~PendingCall();

    QVariant value() const;
    QVariantList values() const;

    int error() const;
    QString errorText() const;

    bool isFinished() const;
    void waitForFinished();

Q_SIGNALS:
    void finished(PendingCall *call);

private:
    enum ReturnType {
        ReturnVoid,
        ReturnString,
        ReturnObjectPath,
        ReturnFileTransferList,
        ReturnTransferWithProperties
    };

    explicit PendingCall(const QDBusPendingCall &call, ReturnType type, QObject *parent = 0);
    explicit PendingCall(Error error, const QString &errorText, QObject *parent = 0);

    class PendingCallPrivate *const d;

    friend class PendingCallPrivate;
    friend class Manager;
    friend class Adapter;
    friend class Device;
    friend class ObexManager;
    friend class ObexTransfer;
    friend class ObexSession;
    friend class ObexObjectPush;
    friend class ObexFileTransfer;
};

} // namespace QBluez

#endif // QBLUEZ_PENDINGCALL_H
