/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * SPDX-FileCopyrightText: 2015 David Rosca <nowrep@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef BLUEZQT_OBEXFILETRANSFERENTRY_H
#define BLUEZQT_OBEXFILETRANSFERENTRY_H

#include <QSharedPointer>
#include <QString>

#include "bluezqt_export.h"

namespace BluezQt
{
/*!
 * \inmodule BluezQt
 * \class BluezQt::ObexFileTransferEntry
 * \inheaderfile BluezQt/ObexFileTransferEntry
 * \brief OBEX file transfer entry.
 *
 * This class represents an entry in the remote file system.
 */
class BLUEZQT_EXPORT ObexFileTransferEntry
{
public:
    /*!
     * \enum BluezQt::ObexFileTransferEntry::Type
     * \brief Type of entry.
     * \value File
     *        Indicates that the entry is a file.
     * \value Folder
     *        Indicates that the entry is a folder.
     * \value Invalid
     *        Indicates that the entry is invalid.
     */
    enum Type {
        File,
        Folder,
        Invalid,
    };

    /*!
     * Creates a new invalid ObexFileTransferEntry object.
     */
    explicit ObexFileTransferEntry();

    virtual ~ObexFileTransferEntry();

    /*!
     * Constructs a new ObexFileTransferEntry object from \a other.
     */
    ObexFileTransferEntry(const ObexFileTransferEntry &other);

    /*!
     * Copies the ObexFileTransferEntry object from \a other.
     */
    ObexFileTransferEntry &operator=(const ObexFileTransferEntry &other);

    /*!
     * Returns whether the entry is valid.
     *
     * This only checks if type() != Invalid.
     */
    bool isValid() const;

    /*!
     * Returns the name of the entry.
     */
    QString name() const;

    /*!
     * Returns the label of the entry.
     */
    QString label() const;

    /*!
     * Returns the type of the entry.
     *
     * The entry can be either a file or folder.
     */
    Type type() const;

    /*!
     * Returns the size of the entry.
     *
     * The size is a number of items in the folder or file size in bytes.
     */
    quint64 size() const;

    /*!
     * Returns the permissions of the entry.
     */
    QString permissions() const;

    /*!
     * Returns memory type where the entry is stored.
     */
    QString memoryType() const;

    /*!
     * Returns the modification time of the entry.
     */
    QDateTime modificationTime() const;

private:
    BLUEZQT_NO_EXPORT explicit ObexFileTransferEntry(const QVariantMap &properties);

    QSharedPointer<class ObexFileTransferEntryPrivate> d;

    friend class PendingCallPrivate;
};

} // namespace BluezQt

Q_DECLARE_METATYPE(BluezQt::ObexFileTransferEntry)

#endif // BLUEZQT_OBEXFILETRANSFERENTRY_H
