/*
 * BluezQt - Asynchronous Bluez wrapper library
 *
 * Copyright (C) 2014 David Rosca <nowrep@gmail.com>
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

#include "obexfiletransfer.h"
#include "pendingcall.h"
#include "utils.h"

#include "obexfiletransfer1.h"

namespace BluezQt
{

typedef org::bluez::obex::FileTransfer1 BluezFileTransfer;

class ObexFileTransferPrivate
{
public:
    ObexFileTransfer *q;
    BluezFileTransfer *m_bluezFileTransfer;
};

ObexFileTransfer::ObexFileTransfer(const QDBusObjectPath &path, QObject *parent)
    : QObject(parent)
    , d(new ObexFileTransferPrivate)
{
    d->m_bluezFileTransfer = new BluezFileTransfer(Strings::orgBluezObex(), path.path(),
                                                   DBusConnection::orgBluezObex(), this);
}

ObexFileTransfer::~ObexFileTransfer()
{
    delete d;
}

QDBusObjectPath ObexFileTransfer::objectPath() const
{
    return QDBusObjectPath(d->m_bluezFileTransfer->path());
}

PendingCall *ObexFileTransfer::changeFolder(const QString &folder)
{
    return new PendingCall(d->m_bluezFileTransfer->ChangeFolder(folder),
                           PendingCall::ReturnVoid, this);
}

PendingCall *ObexFileTransfer::createFolder(const QString &folder)
{
    return new PendingCall(d->m_bluezFileTransfer->CreateFolder(folder),
                           PendingCall::ReturnVoid, this);
}

PendingCall *ObexFileTransfer::listFolder()
{
    return new PendingCall(d->m_bluezFileTransfer->ListFolder(),
                           PendingCall::ReturnFileTransferList, this);
}

PendingCall *ObexFileTransfer::getFile(const QString &targetFileName, const QString &sourceFileName)
{
    return new PendingCall(d->m_bluezFileTransfer->GetFile(targetFileName, sourceFileName),
                           PendingCall::ReturnTransferWithProperties, this);
}

PendingCall *ObexFileTransfer::putFile(const QString &sourceFileName, const QString &targetFileName)
{
    return new PendingCall(d->m_bluezFileTransfer->PutFile(sourceFileName, targetFileName),
                           PendingCall::ReturnTransferWithProperties, this);
}

PendingCall *ObexFileTransfer::copyFile(const QString &sourceFileName, const QString &targetFileName)
{
    return new PendingCall(d->m_bluezFileTransfer->CopyFile(sourceFileName, targetFileName),
                           PendingCall::ReturnVoid, this);
}

PendingCall *ObexFileTransfer::moveFile(const QString &sourceFileName, const QString &targetFileName)
{
    return new PendingCall(d->m_bluezFileTransfer->MoveFile(sourceFileName, targetFileName),
                           PendingCall::ReturnVoid, this);
}

PendingCall *ObexFileTransfer::deleteFile(const QString &fileName)
{
    return new PendingCall(d->m_bluezFileTransfer->Delete(fileName),
                           PendingCall::ReturnVoid, this);
}


} // namespace BluezQt
