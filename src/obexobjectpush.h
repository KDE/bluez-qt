/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * SPDX-FileCopyrightText: 2014-2015 David Rosca <nowrep@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef BLUEZQT_OBEXOBJECTPUSH_H
#define BLUEZQT_OBEXOBJECTPUSH_H

#include <QObject>

#include "bluezqt_export.h"

#include <memory>

class QDBusObjectPath;

namespace BluezQt
{
class PendingCall;

/*!
 * \inmodule BluezQt
 * \class BluezQt::ObexObjectPush
 * \inheaderfile BluezQt/ObexObjectPush
 * \brief OBEX object push.
 *
 * This class represents an OBEX object push interface.
 */
class BLUEZQT_EXPORT ObexObjectPush : public QObject
{
    Q_OBJECT

public:
    /*!
     * Creates a new ObexObjectPush object with the given session \a path as a child of \a parent.
     *
     * This class will be typically used with a \a path
     * from result of ObexManager::createSession().
     */
    explicit ObexObjectPush(const QDBusObjectPath &path, QObject *parent = nullptr);

    ~ObexObjectPush() override;

    /*!
     * Returns the D-Bus object path of the object push session.
     */
    QDBusObjectPath objectPath() const;

    /*!
     * Sends one local file \a fileName to the remote device.
     *
     * The returned ObexTransfer can be used to track progress of transfer.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::InvalidArguments
     * \li PendingCall::Failed
     * \endlist
     *
     * Returns ObexTransfer * pending call.
     */
    PendingCall *sendFile(const QString &fileName);

    /*!
     * Pulls the business card from \targetFileName within a remote device.
     *
     * If an empty \a targetFileName is given, a name will be
     * automatically calculated for the temporary file.
     *
     * The returned ObexTransfer can be used to track progress of transfer.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::InvalidArguments
     * \li PendingCall::Failed
     * \endlist
     *
     * Returns ObexTransfer * pending call.
     */
    PendingCall *pullBusinessCard(const QString &targetFileName);

    /*!
     * Exchanges the business cards \a clientFileName and \a targetFileName on the remote device.
     *
     * This method pushes the local business card to the remote
     * device and then retrieve the remote business card and store
     * it in a local file.
     *
     * If an empty \a targetFileName is given, a name will be
     * automatically calculated for the temporary file.
     *
     * The returned ObexTransfer can be used to track progress of transfer.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::InvalidArguments
     * \li PendingCall::Failed
     * \endlist
     *
     * Returns ObexTransfer * pending call.
     */
    PendingCall *exchangeBusinessCards(const QString &clientFileName, const QString &targetFileName);

private:
    std::unique_ptr<class ObexObjectPushPrivate> const d;

    friend class ObexObjectPushPrivate;
};

} // namespace BluezQt

#endif // BLUEZQT_OBEXOBJECTPUSH_H
