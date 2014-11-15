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

/**
 * OBEX manager.
 *
 * The entry point to communicate with session Bluez obex daemon.
 *
 * You must call init() before other functions can be used.
 *
 * @note If manager is not operational, all methods that returns a PendingCall
 *       will fail with PendingCall::InternalError.
 */
class QBLUEZ_EXPORT ObexManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool initialized READ isInitialized)
    Q_PROPERTY(bool operational READ isOperational NOTIFY operationalChanged)

public:
    /**
     * Creates a new ObexManager object.
     *
     * @param parent
     */
    explicit ObexManager(QObject *parent = 0);

    /**
     * Destroys an ObexManager object.
     */
    ~ObexManager();

    /**
     * Creates a new init job.
     *
     * @return init manager job
     */
    InitObexManagerJob *init();

    /**
     * Returns whether the manager is initialized.
     *
     * @return true if manager is initialized
     */
    bool isInitialized() const;

    /**
     * Returns whether the manager is operational.
     *
     * The manager is operational when initialization was successful
     * and Bluez session daemon is running.
     *
     * @return true if manager is operational
     */
    bool isOperational() const;

    /**
     * Attempts to start org.bluez.obex service by D-Bus activation.
     *
     * Possible return values are 1 if the service was started,
     * 2 if the service is already running or error if the service
     * could not be started.
     *
     * @return quint32 pending call
     */
    static QBluez::PendingCall *startService();

public Q_SLOTS:
    /**
     * Registers agent.
     *
     * This agent will be used to authorize an incoming object push requests.
     *
     * Possible errors: PendingCall::AlreadyExists
     *
     * @param agent agent to be registered
     * @return  void pending call
     */
    PendingCall *registerAgent(ObexAgent *agent);

    /**
     * Unregisters agent.
     *
     * Possible errors: PendingCall::DoesNotExist
     *
     * @param agent agent to be unregistered
     * @return  void pending call
     */
    PendingCall *unregisterAgent(ObexAgent *agent);

    /**
     * Creates a new OBEX session.
     *
     * The @p args parameter is a dictionary to hold optional or
     * type-specific parameters.
     *
     * Typical parameters:
     * <ul>
     *  <li>QString target - type of session to be created</li>
     *  <li>QString source - device address to be used</li>
     * </ul>
     *
     * Supported targets:
     * <ul>
     *   <li>ftp - ObexFileTransfer</li>
     *   <li>map</li>
     *   <li>opp - ObexObjectPush</li>
     *   <li>pbap</li>
     *   <li>sync</li>
     * </ul>
     *
     * Possible errors: PendingCall::InvalidArguments, PendingCall::Failed
     *
     * @param destination address of target device
     * @param args session parameters
     * @return QDBusObjectPath pending call
     */
    PendingCall *createSession(const QString &destination, const QVariantMap &args);

    /**
     * Removes an existing OBEX session.
     *
     * Possible errors: PendingCall::InvalidArguments, PendingCall::NotAuthorized
     *
     * @param session session to be removed
     * @return void pending call
     */
    PendingCall *removeSession(const QDBusObjectPath &session);

Q_SIGNALS:
    /**
     * Indicates that operational state have changed.
     */
    void operationalChanged(bool operational);

    /**
     * Indicates that the session was removed.
     */
    void sessionRemoved(const QDBusObjectPath &session);

private:
    class ObexManagerPrivate *const d;

    friend class ObexManagerPrivate;
    friend class InitObexManagerJobPrivate;
};

} // namespace QBluez

#endif // QBLUEZ_OBEXMANAGER_H
