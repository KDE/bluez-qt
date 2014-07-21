#ifndef QBLUEZ_OBEXOBJECTPUSH_H
#define QBLUEZ_OBEXOBJECTPUSH_H

#include <QObject>

#include "qbluez_export.h"

class QDBusObjectPath;

namespace QBluez
{

class PendingCall;

class QBLUEZ_EXPORT ObexObjectPush : public QObject
{
    Q_OBJECT

public:
    explicit ObexObjectPush(const QDBusObjectPath &path, QObject *parent = 0);
    ~ObexObjectPush();

    // Possible errors: InvalidArguments, Failed
    // Return: ObexTransfer*
    PendingCall *sendFile(const QString &filename);

    // Possible errors: InvalidArguments, Failed
    // Return: ObexTransfer*
    PendingCall *pullBusinessCard(const QString &targetFilename);

    // Possible errors: InvalidArguments, Failed
    // Return: ObexTransfer*
    PendingCall *exchangeBusinessCards(const QString &clientFilename, const QString &targetFilename);

private:
    class ObexObjectPushPrivate *const d;

    friend class ObexObjectPushPrivate;
};

} // namespace QBluez

#endif // QBLUEZ_OBEXOBJECTPUSH_H
