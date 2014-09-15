#ifndef QBLUEZ_OBEXOBJECTPUSH_H
#define QBLUEZ_OBEXOBJECTPUSH_H

#include <QObject>

#include "qbluez_export.h"

class QDBusObjectPath;

namespace QBluez
{

class PendingCall;

/**
 * OBEX object push.
 *
 * This class represents an OBEX object push interface.
 */
class QBLUEZ_EXPORT ObexObjectPush : public QObject
{
    Q_OBJECT

public:
    /**
     * Creates a new ObexObjectPush object.
     *
     * This class will be typically constructed with a
     * @p path from result of ObexManager::createSession().
     *
     * @param path path of session
     * @param parent
     */
    explicit ObexObjectPush(const QDBusObjectPath &path, QObject *parent = 0);

    /**
     * Destroys an ObexObjectPush object.
     */
    ~ObexObjectPush();

    /**
     * D-Bus object path of the object push session.
     *
     * @return object path of session
     */
    QDBusObjectPath objectPath() const;

    /**
     * Sends one local file to the remote device.
     *
     * The returned ObexTransfer can be used to track progress of transfer.
     *
     * Possible errors: PendingCall::InvalidArguments, PendingCall::Failed
     *
     * @param fileName full path of file to send
     * @return ObexTransfer* pending call
     */
    PendingCall *sendFile(const QString &fileName);

    /**
     * Pulls the business card from a remote device.
     *
     * If an empty @p targetFileName is given, a name will be
     * automatically calculated for the temporary file.
     *
     * The returned ObexTransfer can be used to track progress of transfer.
     *
     * Possible errors: PendingCall::InvalidArguments, PendingCall::Failed
     *
     * @param targetFileName full path where the bussiness card will be saved
     * @return ObexTransfer* pending call
     */
    PendingCall *pullBusinessCard(const QString &targetFileName);

    /**
     * Exchanges the business cards on the remote device.
     *
     * This method pushes the local business card to the remote
     * device and then retrieve the remote business card and store
     * it in a local file.
     *
     * If an empty @p targetFileName is given, a name will be
     * automatically calculated for the temporary file.
     *
     * The returned ObexTransfer can be used to track progress of transfer.
     *
     * Possible errors: PendingCall::InvalidArguments, PendingCall::Failed
     *
     * @param clientFileName full path to local business card
     * @param targetFileName full path where the bussiness card will be saved
     * @return ObexTransfer* pending call
     */
    PendingCall *exchangeBusinessCards(const QString &clientFileName, const QString &targetFileName);

private:
    class ObexObjectPushPrivate *const d;

    friend class ObexObjectPushPrivate;
};

} // namespace QBluez

#endif // QBLUEZ_OBEXOBJECTPUSH_H
