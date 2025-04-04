/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * SPDX-FileCopyrightText: 2014-2015 David Rosca <nowrep@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef BLUEZQT_PENDINGCALL_H
#define BLUEZQT_PENDINGCALL_H

#include <functional>

#include <QObject>

#include "bluezqt_export.h"

#include <memory>

class QDBusError;
class QDBusPendingCall;
class QDBusPendingCallWatcher;

namespace BluezQt
{
/*!
 * \inmodule BluezQt
 * \class BluezQt::PendingCall
 * \inheaderfile BluezQt/PendingCall
 * \brief Pending method call.
 *
 * This class represents a pending method call. It is a convenient wrapper
 * around QDBusPendingReply and QDBusPendingCallWatcher.
 */
class BLUEZQT_EXPORT PendingCall : public QObject
{
    Q_OBJECT

    /*! \property BluezQt::PendingCall::value */
    Q_PROPERTY(QVariant value READ value)
    /*! \property BluezQt::PendingCall::values */
    Q_PROPERTY(QVariantList values READ values)
    /*! \property BluezQt::PendingCall::error */
    Q_PROPERTY(int error READ error)
    /*! \property BluezQt::PendingCall::errorText */
    Q_PROPERTY(QString errorText READ errorText)
    /*! \property BluezQt::PendingCall::isFinished */
    Q_PROPERTY(bool isFinished READ isFinished)
    /*! \property BluezQt::PendingCall::userData */
    Q_PROPERTY(QVariant userData READ userData WRITE setUserData)

public:
    /*!
     * \enum BluezQt::PendingCall::Error
     * \brief Known error types.
     * \value NoError
     *        Indicates there is no error.
     * \value NotReady
     *        Indicates that the device is not ready.
     * \value Failed
     *        Indicates that the action have failed.
     * \value Rejected
     *        Indicates that the action was rejected.
     * \value Canceled
     *        Indicates that the action was canceled.
     * \value InvalidArguments
     *        Indicates that invalid arguments were passed.
     * \value AlreadyExists
     *        Indicates that an agent or pairing record already exists.
     * \value DoesNotExist
     *        Indicates that an agent, service or pairing operation does not exists.
     * \value InProgress
     *        Indicates that the action is already in progress.
     * \value NotInProgress
     *        Indicates that the action is not in progress.
     * \value AlreadyConnected
     *        Indicates that the device is already connected.
     * \value ConnectFailed
     *        Indicates that the connection to the device have failed.
     * \value NotConnected
     *        Indicates that the device is not connected.
     * \value NotSupported
     *        Indicates that the action is not supported.
     * \value NotAuthorized
     *        Indicates that the caller is not authorized to do the action.
     * \value AuthenticationCanceled
     *        Indicates that the authentication was canceled.
     * \value AuthenticationFailed
     *        Indicates that the authentication have failed.
     * \value AuthenticationRejected
     *        Indicates that the authentication was rejected.
     * \value AuthenticationTimeout
     *        Indicates that the authentication timed out.
     * \value ConnectionAttemptFailed
     *        Indicates that the connection attempt have failed.
     * \value InvalidLength
     *        Indicates that the data provided generates a data packet which is too long.
     * \value NotPermitted
     *        Indicates that the action is not permitted (e.g. maximum reached or socket locked).
     * \value DBusError
     *        Indicates an error with D-Bus.
     * \value InternalError
     *        Indicates an internal error.
     * \value UnknownError
     *        Indicates an unknown error.
     */
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
        NotInProgress = 9,
        AlreadyConnected = 10,
        ConnectFailed = 11,
        NotConnected = 12,
        NotSupported = 13,
        NotAuthorized = 14,
        AuthenticationCanceled = 15,
        AuthenticationFailed = 16,
        AuthenticationRejected = 17,
        AuthenticationTimeout = 18,
        ConnectionAttemptFailed = 19,
        InvalidLength = 20,
        NotPermitted = 21,
        DBusError = 98,
        InternalError = 99,
        UnknownError = 100,
    };
    Q_ENUM(Error)

    ~PendingCall() override;

    /*!
     * Returns a first return value of the call.
     */
    QVariant value() const;

    /*!
     * Returns all values of the call.
     */
    QVariantList values() const;

    /*!
     * Returns an error code.
     * \sa Error
     */
    int error() const;

    /*!
     * Returns an error text.
     */
    QString errorText() const;

    /*!
     * Returns whether the call is finished.
     */
    bool isFinished() const;

    /*!
     * Waits for the call to finish.
     * \warning This method blocks until the call finishes!
     */
    void waitForFinished();

    /*!
     * Returns the user data of the call.
     */
    QVariant userData() const;

    /*!
     * Sets the \a userData of the call.
     */
    void setUserData(const QVariant &userData);

Q_SIGNALS:
    /*!
     * Indicates that the \a call has finished.
     */
    void finished(PendingCall *call);

private:
    enum ReturnType {
        ReturnVoid,
        ReturnUint32,
        ReturnString,
        ReturnStringList,
        ReturnObjectPath,
        ReturnFileTransferList,
        ReturnTransferWithProperties,
        ReturnByteArray
    };

    BLUEZQT_NO_EXPORT explicit PendingCall(Error error, const QString &errorText, QObject *parent = nullptr);
    BLUEZQT_NO_EXPORT explicit PendingCall(const QDBusPendingCall &call, ReturnType type, QObject *parent = nullptr);

    // exported because called from template BluezQt::TPendingCall constructor
    using ErrorProcessor = std::function<void(const QDBusError &error)>;
    using ExternalProcessor = std::function<void(QDBusPendingCallWatcher *watcher, ErrorProcessor errorProcessor, QVariantList *values)>;
    explicit PendingCall(const QDBusPendingCall &call, ExternalProcessor externalProcessor, QObject *parent = nullptr);

    std::unique_ptr<class PendingCallPrivate> const d;

    friend class PendingCallPrivate;
    friend class Manager;
    friend class Adapter;
    friend class GattServiceRemote;
    friend class GattCharacteristicRemote;
    friend class GattDescriptorRemote;
    friend class Device;
    friend class GattManager;
    friend class LEAdvertisingManager;
    friend class Media;
    friend class MediaPlayer;
    friend class MediaTransport;
    friend class ObexManager;
    friend class ObexTransfer;
    friend class ObexSession;
    friend class ObexObjectPush;
    friend class ObexFileTransfer;
    template<class... T>
    friend class TPendingCall;
};

} // namespace BluezQt

#endif // BLUEZQT_PENDINGCALL_H
