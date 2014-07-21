#ifndef QBLUEZ_OBEXTRANSFER_H
#define QBLUEZ_OBEXTRANSFER_H

#include <QObject>

#include "qbluez_export.h"

namespace QBluez
{

class PendingCall;
class ObexSession;

class QBLUEZ_EXPORT ObexTransfer : public QObject
{
    Q_OBJECT

    Q_ENUMS(Status)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(ObexSession* session READ session)
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString type READ type)
    Q_PROPERTY(quint64 time READ time)
    Q_PROPERTY(quint64 size READ size)
    Q_PROPERTY(quint64 transferred READ transferred NOTIFY transferredChanged)
    Q_PROPERTY(QString fileName READ fileName)
    Q_PROPERTY(bool suspendable READ isSuspendable)

public:
    enum Status {
        Queued,
        Active,
        Suspended,
        Complete,
        Error,
        Unknown
    };

    ~ObexTransfer();

    Status status() const;

    ObexSession *session() const;

    QString name() const;

    QString type() const;

    quint64 time() const;

    quint64 size() const;

    quint64 transferred() const;

    QString fileName() const;

    bool isSuspendable() const;

    // Possible errors: NotAuthorized, InProgress, Failed
    PendingCall *cancel();

    // Possible errors: NotAuthorized, NotInProgress
    PendingCall *suspend();

    // Possible errors: NotAuthorized, NotInProgress
    PendingCall *resume();

Q_SIGNALS:
    void statusChanged(Status status);
    void transferredChanged(quint64 transferred);

private:
    explicit ObexTransfer(const QString &path, QObject *parent = 0);

    class ObexTransferPrivate *const d;

    friend class ObexTransferPrivate;
    friend class ObexAgentAdaptor;
    friend class PendingCall;
};

} // namespace QBluez

#endif // QBLUEZ_OBEXTRANSFER_H
