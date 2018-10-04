/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * Copyright (C) 2014-2015 David Rosca <nowrep@gmail.com>
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

#ifndef BLUEZQT_PENDINGCALL_H
#define BLUEZQT_PENDINGCALL_H

#include <QObject>

#include "bluezqt_export.h"

class QDBusPendingCall;

namespace BluezQt
{

/**
 * @class BluezQt::PendingCall pendingcall.h <BluezQt/PendingCall>
 *
 * Pending method call.
 *
 * This class represents a pending method call. It is a convenient wrapper
 * around QDBusPendingReply and QDBusPendingCallWatcher.
 */
class BLUEZQT_EXPORT PendingCall : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariant value READ value)
    Q_PROPERTY(QVariantList values READ values)
    Q_PROPERTY(int error READ error)
    Q_PROPERTY(QString errorText READ errorText)
    Q_PROPERTY(bool isFinished READ isFinished)
    Q_PROPERTY(QVariant userData READ userData WRITE setUserData)

public:
    /**
     * Known error types.
     */
    enum Error {
        /** Indicates there is no error. */
        NoError = 0,
        /** Indicates that the device is not ready. */
        NotReady = 1,
        /** Indicates that the action have failed. */
        Failed = 2,
        /** Indicates that the action was rejected. */
        Rejected = 3,
        /** Indicates that the action was canceled. */
        Canceled = 4,
        /** Indicates that invalid arguments were passed. */
        InvalidArguments = 5,
        /** Indicates that an agent or pairing record already exists. */
        AlreadyExists = 6,
        /** Indicates that an agent, service or pairing operation does not exists. */
        DoesNotExist = 7,
        /** Indicates that the action is already in progress. */
        InProgress = 8,
        /** Indicates that the action is not in progress. */
        NotInProgress = 9,
        /** Indicates that the device is already connected. */
        AlreadyConnected = 10,
        /** Indicates that the connection to the device have failed. */
        ConnectFailed = 11,
        /** Indicates that the device is not connected. */
        NotConnected = 12,
        /** Indicates that the action is not supported. */
        NotSupported = 13,
        /** Indicates that the caller is not authorized to do the action. */
        NotAuthorized = 14,
        /** Indicates that the authentication was canceled. */
        AuthenticationCanceled = 15,
        /** Indicates that the authentication have failed. */
        AuthenticationFailed = 16,
        /** Indicates that the authentication was rejected. */
        AuthenticationRejected = 17,
        /** Indicates that the authentication timed out. */
        AuthenticationTimeout = 18,
        /** Indicates that the connection attempt have failed. */
        ConnectionAttemptFailed = 19,
        /** Indicates an error with D-Bus. */
        DBusError = 98,
        /** Indicates an internal error. */
        InternalError = 99,
        /** Indicates an unknown error. */
        UnknownError = 100
    };
    Q_ENUM(Error)

    /**
     * Destroys a PendingCall object.
     */
    ~PendingCall();

    /**
     * Returns a first return value of the call.
     *
     * @return first return value
     */
    QVariant value() const;

    /**
     * Returns all values of the call.
     *
     * @return all return values
     */
    QVariantList values() const;

    /**
     * Returns an error code.
     *
     * @return error code
     * @see Error
     */
    int error() const;

    /**
     * Returns an error text.
     *
     * @return error text
     */
    QString errorText() const;

    /**
     * Returns whether the call is finished.
     *
     * @return true if call is finished
     */
    bool isFinished() const;

    /**
     * Waits for the call to finish.
     *
     * @warning This method blocks until the call finishes!
     */
    void waitForFinished();

    /**
     * Returns the user data of the call.
     *
     * @return user data of call
     */
    QVariant userData() const;

    /**
     * Sets the user data of the call.
     *
     * @param userData user data
     */
    void setUserData(const QVariant &userData);

Q_SIGNALS:
    /**
     * Indicates that the call have finished.
     */
    void finished(PendingCall *call);

private:
    enum ReturnType {
        ReturnVoid,
        ReturnUint32,
        ReturnString,
        ReturnObjectPath,
        ReturnFileTransferList,
        ReturnTransferWithProperties
    };

    explicit PendingCall(const QDBusPendingCall &call, ReturnType type, QObject *parent = nullptr);
    explicit PendingCall(Error error, const QString &errorText, QObject *parent = nullptr);

    class PendingCallPrivate *const d;

    friend class PendingCallPrivate;
    friend class Manager;
    friend class Adapter;
    friend class Device;
    friend class Media;
    friend class MediaPlayer;
    friend class ObexManager;
    friend class ObexTransfer;
    friend class ObexSession;
    friend class ObexObjectPush;
    friend class ObexFileTransfer;
};

} // namespace BluezQt

#endif // BLUEZQT_PENDINGCALL_H
