#ifndef QBLUEZ_OBEXSESSION_H
#define QBLUEZ_OBEXSESSION_H

#include <QObject>

#include "qbluez_export.h"

namespace QBluez
{

class PendingCall;

/**
 * OBEX session.
 *
 * This class represents an OBEX session.
 */
class QBLUEZ_EXPORT ObexSession : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString source READ source)
    Q_PROPERTY(QString destination READ destination)
    Q_PROPERTY(quint8 channel READ channel)
    Q_PROPERTY(QString target READ target)
    Q_PROPERTY(QString root READ root)

public:
    /**
     * Destroys an ObexSession object.
     */
    ~ObexSession();

    /**
     * Returns address of the Bluetooth adapter.
     *
     * @see Manager::adapterForAddress()
     *
     * @return address of adapter
     */
    QString source() const;

    /**
     * Returns address of the Bluetooth device.
     *
     * @see Manager::deviceForAddress(), Adapter::deviceForAddress()
     *
     * @return address of device
     */
    QString destination() const;

    /**
     * Returns the Bluetooth channel.
     *
     * @return channel
     */
    quint8 channel() const;

    /**
     * Returns the target UUID.
     *
     * @return target UUID
     */
    QString target() const;

    /**
     * Returns the root path.
     *
     * @return root path
     */
    QString root() const;

    /**
     * Returns the remote device capabilities.
     *
     * Possible errors: PendingCall::NotSupported, PendingCall::Failed
     *
     * @return QString pending call
     */
    PendingCall *getCapabilities();

private:
    explicit ObexSession(const QString &path, QObject *parent = 0);

    class ObexSessionPrivate *const d;

    friend class ObexSessionPrivate;
    friend class ObexTransferPrivate;
};

} // namespace QBluez

#endif // QBLUEZ_OBEXSESSION_H
