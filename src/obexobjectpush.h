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

#ifndef BLUEZQT_OBEXOBJECTPUSH_H
#define BLUEZQT_OBEXOBJECTPUSH_H

#include <QObject>

#include "bluezqt_export.h"

class QDBusObjectPath;

namespace BluezQt
{

class PendingCall;

/**
 * @class BluezQt::ObexObjectPush obexobjectpush.h <BluezQt/ObexObjectPush>
 *
 * OBEX object push.
 *
 * This class represents an OBEX object push interface.
 */
class BLUEZQT_EXPORT ObexObjectPush : public QObject
{
    Q_OBJECT

public:
    /**
     * Creates a new ObexObjectPush object.
     *
     * This class will be typically used with a @p path
     * from result of ObexManager::createSession().
     *
     * @param path path of session
     * @param parent
     */
    explicit ObexObjectPush(const QDBusObjectPath &path, QObject *parent = nullptr);

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
     * @return ObexTransfer * pending call
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
     * @return ObexTransfer * pending call
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
     * @return ObexTransfer * pending call
     */
    PendingCall *exchangeBusinessCards(const QString &clientFileName, const QString &targetFileName);

private:
    class ObexObjectPushPrivate *const d;

    friend class ObexObjectPushPrivate;
};

} // namespace BluezQt

#endif // BLUEZQT_OBEXOBJECTPUSH_H
