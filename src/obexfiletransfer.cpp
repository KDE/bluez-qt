#include "obexfiletransfer.h"
#include "pendingcall.h"
#include "utils_p.h"

#include "obexfiletransfer1.h"

namespace QBluez
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
                                                   QDBusConnection::sessionBus(), this);
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


} // namespace QBluez
