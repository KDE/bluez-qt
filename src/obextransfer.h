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

#ifndef BLUEZQT_OBEXTRANSFER_H
#define BLUEZQT_OBEXTRANSFER_H

#include <QObject>

#include "types.h"
#include "bluezqt_export.h"

class QDBusObjectPath;

namespace BluezQt
{

class PendingCall;

/**
 * @class BluezQt::ObexTransfer obextransfer.h <BluezQt/ObexTransfer>
 *
 * OBEX transfer.
 *
 * This class represents transfer of one file.
 */
class BLUEZQT_EXPORT ObexTransfer : public QObject
{
    Q_OBJECT

    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString type READ type)
    Q_PROPERTY(quint64 time READ time)
    Q_PROPERTY(quint64 size READ size)
    Q_PROPERTY(quint64 transferred READ transferred NOTIFY transferredChanged)
    Q_PROPERTY(QString fileName READ fileName NOTIFY fileNameChanged)
    Q_PROPERTY(bool suspendable READ isSuspendable)

public:
    /**
     * Status of transfer.
     */
    enum Status {
        /** Indicates that the transfer is queued. */
        Queued,
        /** Indicates that the transfer is active. */
        Active,
        /** Indicates that the transfer is suspended. */
        Suspended,
        /** Indicates that the transfer have completed successfuly. */
        Complete,
        /** Indicates that the transfer have failed with error. */
        Error,
        /** Indicates that the transfer status is unknown. */
        Unknown
    };
    Q_ENUM(Status)

    /**
     * Destroys an ObexTransfer object.
     */
    ~ObexTransfer();

    /**
     * Returns a shared pointer from this.
     *
     * @return ObexTransferPtr
     */
    ObexTransferPtr toSharedPtr() const;

    /**
     * D-Bus object path of the transfer.
     *
     * @return object path of transfer
     */
    QDBusObjectPath objectPath() const;

    /**
     * Returns the status of the transfer.
     *
     * @return status of transfer
     */
    Status status() const;

    /**
     * Returns the name of the transferred object.
     *
     * @return name of transferred object
     */
    QString name() const;

    /**
     * Returns the type of the transferred object.
     *
     * @return type of transferred object
     */
    QString type() const;

    /**
     * Returns the time of the transferred object.
     *
     * @return time of transferred object
     */
    quint64 time() const;

    /**
     * Returns the total size of the transferred object.
     *
     * @return size of transferred object
     */
    quint64 size() const;

    /**
     * Returns the number of bytes transferred.
     *
     * @return number of bytes transferred
     */
    quint64 transferred() const;

    /**
     * Returns the full name of the transferred file.
     *
     * @return full name of transferred file
     */
    QString fileName() const;

    /**
     * Returns whether the transfer is suspendable.
     *
     * @return true if transfer is suspendable
     */
    bool isSuspendable() const;

    /**
     * Stops the current transfer.
     *
     * Possible errors: PendingCall::NotAuthorized, PendingCall::InProgress
     *                  PendingCall::Failed
     *
     * @return void pending call
     */
    PendingCall *cancel();

    /**
     * Suspends the current transfer.
     *
     * Only suspendable transfers can be suspended.
     *
     * Possible errors: PendingCall::NotAuthorized, PendingCall::NotInProgress
     *
     * @see isSuspendable() const
     *
     * @return void pending call
     */
    PendingCall *suspend();

    /**
     * Resumes the current transfer.
     *
     * Possible errors: PendingCall::NotAuthorized, PendingCall::NotInProgress
     *
     * @return void pending call
     */
    PendingCall *resume();

Q_SIGNALS:
    /**
     * Indicates that the status of transfer have changed.
     */
    void statusChanged(Status status);

    /**
     * Indicates that the number of transferred bytes have changed.
     */
    void transferredChanged(quint64 transferred);

    /**
     * Indicates that the name of transferred file have changed.
     */
    void fileNameChanged(const QString &fileName);

private:
    explicit ObexTransfer(const QString &path, const QVariantMap &properties);

    class ObexTransferPrivate *const d;

    friend class ObexTransferPrivate;
    friend class ObexAgentAdaptor;
    friend class PendingCallPrivate;
};

} // namespace BluezQt

#endif // BLUEZQT_OBEXTRANSFER_H
