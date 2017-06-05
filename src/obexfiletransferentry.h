/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * Copyright (C) 2015 David Rosca <nowrep@gmail.com>
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

#ifndef BLUEZQT_OBEXFILETRANSFERENTRY_H
#define BLUEZQT_OBEXFILETRANSFERENTRY_H

#include <QString>
#include <QSharedPointer>

#include "bluezqt_export.h"

namespace BluezQt
{

/**
 * @class BluezQt::ObexFileTransferEntry obexfiletransferentry.h <BluezQt/ObexFileTransferEntry>
 *
 * OBEX file transfer entry.
 *
 * This class represents an entry in remote file system.
 */
class BLUEZQT_EXPORT ObexFileTransferEntry
{
public:
    /** Type of entry. */
    enum Type {
        /** Indicates that the entry is a file. */
        File,
        /** Indicates that the entry is a folder. */
        Folder,
        /** Indicates that the entry is invalid. */
        Invalid
    };

    /**
     * Creates a new invalid ObexFileTransferEntry object.
     */
    explicit ObexFileTransferEntry();

    /**
     * Destroys an ObexFileTransferEntry object.
     */
    virtual ~ObexFileTransferEntry();

    /**
     * Copy constructor.
     *
     * @param other
     */
    ObexFileTransferEntry(const ObexFileTransferEntry &other);

    /**
     * Copy assignment operator.
     *
     * @param other
     */
    ObexFileTransferEntry &operator=(const ObexFileTransferEntry &other);

    /**
     * Returns whether the entry is valid.
     *
     * This only checks if type() != Invalid.
     *
     * @return true if entry is valid
     */
    bool isValid() const;

    /**
     * Returns a name of the entry.
     *
     * @return name of entry
     */
    QString name() const;

    /**
     * Returns a label of the entry.
     *
     * @return label of entry
     */
    QString label() const;

    /**
     * Returns a type of the entry.
     *
     * Entry can be either a file or folder.
     *
     * @return type of entry
     */
    Type type() const;

    /**
     * Returns a size of the entry.
     *
     * Size is a number of items in the folder or file size in bytes.
     *
     * @return size of entry
     */
    quint64 size() const;

    /**
     * Returns a permissions of the entry.
     *
     * @return permissions of entry
     */
    QString permissions() const;

    /**
     * Returns a memory type where the entry is stored.
     *
     * @return memory type
     */
    QString memoryType() const;

    /**
     * Returns a modification time of the entry.
     *
     * @return modification time of entry
     */
    QDateTime modificationTime() const;

private:
    explicit ObexFileTransferEntry(const QVariantMap &properties);

    QSharedPointer<class ObexFileTransferEntryPrivate> d;

    friend class PendingCallPrivate;
};

} // namespace BluezQt

Q_DECLARE_METATYPE(BluezQt::ObexFileTransferEntry)

#endif // BLUEZQT_OBEXFILETRANSFERENTRY_H
