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

#ifndef BLUEZQT_OBEXFILETRANSFER_H
#define BLUEZQT_OBEXFILETRANSFER_H

#include <QObject>

#include "obexfiletransferentry.h"
#include "bluezqt_export.h"

class QDBusObjectPath;

namespace BluezQt
{

class PendingCall;

/**
 * OBEX file transfer.
 *
 * This class represents an OBEX file transfer interface.
 */
class BLUEZQT_EXPORT ObexFileTransfer : public QObject
{
    Q_OBJECT

public:
    /**
     * Creates a new ObexFileTransfer object.
     *
     * This class will be typically used with a @p path
     * from result of ObexManager::createSession().
     *
     * @param path path of session
     * @param parent
     */
    explicit ObexFileTransfer(const QDBusObjectPath &path, QObject *parent = Q_NULLPTR);

    /**
     * Destroys an ObexFileTransfer object.
     */
    ~ObexFileTransfer();

    /**
     * D-Bus object path of the file transfer session.
     *
     * @return object path of session
     */
    QDBusObjectPath objectPath() const;

    /**
     * Changes the current folder.
     *
     * Possible errors: PendingCall::InvalidArguments, PendingCall::Failed
     *
     * @param folder folder to be changed
     * @return void pending call
     */
    PendingCall *changeFolder(const QString &folder);

    /**
     * Creates a new folder.
     *
     * Possible errors: PendingCall::InvalidArguments, PendingCall::Failed
     *
     * @param folder name of new folder
     * @return void pending call
     */
    PendingCall *createFolder(const QString &folder);

    /**
     * Lists a current folder.
     *
     * Possible errors: PendingCall::Failed
     *
     * @return QList<ObexFileTransferEntry> pending call
     */
    PendingCall *listFolder();

    /**
     * Gets the file from the remote device.
     *
     * If an empty @p targetFileName is given, a name will be
     * automatically calculated for the temporary file.
     *
     * Possible errors: PendingCall::InvalidArguments, PendingCall::Failed
     *
     * @param targetFileName full local path where the file will be saved
     * @param sourceFileName file within the remote device
     * @return ObexTransferPtr pending call
     */
    PendingCall *getFile(const QString &targetFileName, const QString &sourceFileName);

    /**
     * Puts the file to the remote device.
     *
     * If an empty @p targetFileName is given, a name will be
     * automatically calculated for the temporary file.
     *
     * Possible errors: PendingCall::InvalidArguments, PendingCall::Failed
     *
     * @param sourceFileName full path of the local file
     * @param targetFileName file to be saved within the remote device
     * @return ObexTransferPtr pending call
     */
    PendingCall *putFile(const QString &sourceFileName, const QString &targetFileName);

    /**
     * Copies a file within the remote device.
     *
     * Possible errors: PendingCall::InvalidArguments, PendingCall::Failed
     *
     * @param sourceFileName source within the remote device
     * @param targetFileName target file within the remote device
     * @return void pending call
     */
    PendingCall *copyFile(const QString &sourceFileName, const QString &targetFileName);

    /**
     * Moves a file within the remote device.
     *
     * Possible errors: PendingCall::InvalidArguments, PendingCall::Failed
     *
     * @param sourceFileName source file within the remote device
     * @param targetFileName target file within the remote device
     * @return void pending call
     */
    PendingCall *moveFile(const QString &sourceFileName, const QString &targetFileName);

    /**
     * Deletes a file/folder within the remote device.
     *
     * Possible errors: PendingCall::InvalidArguments, PendingCall::Failed
     *
     * @param fileName file within the remote device
     * @return void pending call
     */
    PendingCall *deleteFile(const QString &fileName);

private:
    class ObexFileTransferPrivate *const d;

    friend class ObexFileTransferPrivate;
};

} // namespace BluezQt

#endif // BLUEZQT_OBEXFILETRANSFER_H
