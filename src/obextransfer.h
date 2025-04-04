/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * SPDX-FileCopyrightText: 2014-2015 David Rosca <nowrep@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef BLUEZQT_OBEXTRANSFER_H
#define BLUEZQT_OBEXTRANSFER_H

#include <QObject>

#include "bluezqt_export.h"
#include "types.h"

#include <memory>

class QDBusObjectPath;

namespace BluezQt
{
class PendingCall;

/*!
 * \inmodule BluezQt
 * \class BluezQt::ObexTransfer
 * \inheaderfile BluezQt/ObexTransfer
 * \brief OBEX transfer.
 *
 * This class represents transfer of one file.
 */
class BLUEZQT_EXPORT ObexTransfer : public QObject
{
    Q_OBJECT

    /*! \property BluezQt::ObexTransfer::status */
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    /*! \property BluezQt::ObexTransfer::name */
    Q_PROPERTY(QString name READ name)
    /*! \property BluezQt::ObexTransfer::type */
    Q_PROPERTY(QString type READ type)
    /*! \property BluezQt::ObexTransfer::time */
    Q_PROPERTY(quint64 time READ time)
    /*! \property BluezQt::ObexTransfer::size */
    Q_PROPERTY(quint64 size READ size)
    /*! \property BluezQt::ObexTransfer::transferred */
    Q_PROPERTY(quint64 transferred READ transferred NOTIFY transferredChanged)
    /*! \property BluezQt::ObexTransfer::fileName */
    Q_PROPERTY(QString fileName READ fileName NOTIFY fileNameChanged)
    /*! \property BluezQt::ObexTransfer::suspendable */
    Q_PROPERTY(bool suspendable READ isSuspendable)

public:
    /*!
     * \enum BluezQt::ObexTransfer::Status
     * \brief Transfer status.
     * \value Queued
     *        Indicates that the transfer is queued.
     * \value Active
     *        Indicates that the transfer is active.
     * \value Suspended
     *        Indicates that the transfer is suspended.
     * \value Complete
     *        Indicates that the transfer have completed successfully.
     * \value Error
     *        Indicates that the transfer have failed with error.
     * \value Unknown
     *        Indicates that the transfer status is unknown.
     */
    enum Status {
        Queued,
        Active,
        Suspended,
        Complete,
        Error,
        Unknown,
    };
    Q_ENUM(Status)

    ~ObexTransfer() override;

    /*!
     * Returns a shared pointer from this.
     */
    ObexTransferPtr toSharedPtr() const;

    /*!
     * Returns the D-Bus object path of the transfer.
     */
    QDBusObjectPath objectPath() const;

    /*!
     * Returns the status of the transfer.
     */
    Status status() const;

    /*!
     * Returns the name of the transferred object.
     */
    QString name() const;

    /*!
     * Returns the type of the transferred object.
     */
    QString type() const;

    /*!
     * Returns the time of the transferred object.
     */
    quint64 time() const;

    /*!
     * Returns the total size of the transferred object.
     */
    quint64 size() const;

    /*!
     * Returns the number of bytes transferred.
     */
    quint64 transferred() const;

    /*!
     * Returns the full name of the transferred file.
     */
    QString fileName() const;

    /*!
     * Returns whether the transfer is suspendable.
     */
    bool isSuspendable() const;

    /*!
     * Stops the current transfer.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::NotAuthorized
     * \li PendingCall::InProgress
     * \li PendingCall::Failed
     * \endlist
     *
     * Returns void pending call.
     */
    PendingCall *cancel();

    /*!
     * Suspends the current transfer.
     *
     * Only suspendable transfers can be suspended.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::NotAuthorized
     * \li PendingCall::NotInProgress
     * \endlist
     *
     * \sa isSuspendable()
     *
     * Returns void pending call.
     */
    PendingCall *suspend();

    /*!
     * Resumes the current transfer.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::NotAuthorized
     * \li PendingCall::NotInProgress
     * \endlist
     *
     * Returns void pending call.
     */
    PendingCall *resume();

Q_SIGNALS:
    /*!
     * Indicates that the \a status of transfer has changed.
     */
    void statusChanged(Status status);

    /*!
     * Indicates that the number of \a transferred bytes has changed.
     */
    void transferredChanged(quint64 transferred);

    /*!
     * Indicates that the transferred \a fileName has changed.
     */
    void fileNameChanged(const QString &fileName);

private:
    BLUEZQT_NO_EXPORT explicit ObexTransfer(const QString &path, const QVariantMap &properties);

    std::unique_ptr<class ObexTransferPrivate> const d;

    friend class ObexTransferPrivate;
    friend class ObexAgentAdaptor;
    friend class PendingCallPrivate;
};

} // namespace BluezQt

#endif // BLUEZQT_OBEXTRANSFER_H
