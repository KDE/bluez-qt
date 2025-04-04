/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * SPDX-FileCopyrightText: 2014-2015 David Rosca <nowrep@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef BLUEZQT_OBEXFILETRANSFER_H
#define BLUEZQT_OBEXFILETRANSFER_H

#include <QObject>

#include "bluezqt_export.h"
#include "obexfiletransferentry.h"

#include <memory>

class QDBusObjectPath;

namespace BluezQt
{
class PendingCall;

/*!
 * \inmodule BluezQt
 * \class BluezQt::ObexFileTransfer
 * \inheaderfile BluezQt/ObexFileTransfer
 * \brief OBEX file transfer.
 *
 * This class represents an OBEX file transfer interface.
 */
class BLUEZQT_EXPORT ObexFileTransfer : public QObject
{
    Q_OBJECT

public:
    /*!
     * Creates a new ObexFileTransfer object at the given session \a path as a child of \a parent.
     *
     * This class will be typically used with a \a path
     * from result of ObexManager::createSession().
     */
    explicit ObexFileTransfer(const QDBusObjectPath &path, QObject *parent = nullptr);

    ~ObexFileTransfer() override;

    /*!
     * Returns the D-Bus object path of the file transfer session.
     */
    QDBusObjectPath objectPath() const;

    /*!
     * Changes the current \a folder.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::InvalidArguments
     * \li PendingCall::Failed
     * \endlist
     *
     * Returns void pending call.
     */
    PendingCall *changeFolder(const QString &folder);

    /*!
     * Creates a new \a folder.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::InvalidArguments
     * \li PendingCall::Failed
     * \endlist
     *
     * Returns void pending call.
     */
    PendingCall *createFolder(const QString &folder);

    /*!
     * Lists a current folder.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::Failed
     * \endlist
     *
     * Returns QList<ObexFileTransferEntry> pending call.
     */
    PendingCall *listFolder();

    /*!
     * Gets the file at full path \a sourceFileName from the remote device and saves it to \a targetFileName.
     *
     * If an empty \a targetFileName is given, a name will be
     * automatically calculated for the temporary file.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::InvalidArguments
     * \li PendingCall::Failed
     * \endlist
     *
     * Returns ObexTransferPtr pending call.
     */
    PendingCall *getFile(const QString &targetFileName, const QString &sourceFileName);

    /*!
     * Puts the file at full path \a sourceFileName to the remote device in \a targetFileName.
     *
     * If an empty \a targetFileName is given, a name will be
     * automatically calculated for the temporary file.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::InvalidArguments
     * \li PendingCall::Failed
     * \endlist
     *
     * Returns ObexTransferPtr pending call.
     */
    PendingCall *putFile(const QString &sourceFileName, const QString &targetFileName);

    /*!
     * Copies a file at \a sourceFileName to \a targetFileName, both within the remote device.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::InvalidArguments
     * \li PendingCall::Failed
     * \endlist
     *
     * Returns void pending call.
     */
    PendingCall *copyFile(const QString &sourceFileName, const QString &targetFileName);

    /*!
     * Moves a file \a sourceFileName to \a targetFileName, both within the remote device.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::InvalidArguments
     * \li PendingCall::Failed
     * \endlist
     *
     * Returns void pending call.
     */
    PendingCall *moveFile(const QString &sourceFileName, const QString &targetFileName);

    /*!
     * Deletes a file/folder at \a fileName within the remote device.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::InvalidArguments
     * \li PendingCall::Failed
     * \endlist
     *
     * Returns void pending call.
     */
    PendingCall *deleteFile(const QString &fileName);

private:
    std::unique_ptr<class ObexFileTransferPrivate> const d;

    friend class ObexFileTransferPrivate;
};

} // namespace BluezQt

#endif // BLUEZQT_OBEXFILETRANSFER_H
