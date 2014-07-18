#ifndef QBLUEZ_OBEXMANAGER_H
#define QBLUEZ_OBEXMANAGER_H

#include <QObject>

#include "qbluez_export.h"

class QDBusObjectPath;

namespace QBluez
{

class ObexAgent;
class PendingCall;
class InitObexManagerJob;

class QBLUEZ_EXPORT ObexManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool initialized READ isInitialized)
    Q_PROPERTY(bool operational READ isOperational NOTIFY operationalChanged)

public:
    explicit ObexManager(QObject *parent = 0);
    ~ObexManager();

    InitObexManagerJob *init();

    bool isInitialized() const;
    bool isOperational() const;

    // Possible errors: AlreadyExists
    PendingCall *registerAgent(ObexAgent *agent);

    // Possible errors: DoesNotExist
    PendingCall *unregisterAgent(ObexAgent *agent);

    // Possible errors: InvalidArguments, Failed
    // Return: ObexSession*
    PendingCall *createSession(const QString &destination, const QVariantMap &args);

    // Possible errors: InvalidArguments, NotAuthorized
    PendingCall *removeSession(const QDBusObjectPath &session);

Q_SIGNALS:
    void operationalChanged(bool operational);

private:
    class ObexManagerPrivate *const d;

    friend class ObexManagerPrivate;
    friend class InitObexManagerJobPrivate;
};

} // namespace QBluez

#endif // QBLUEZ_OBEXMANAGER_H
