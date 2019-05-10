#include "obexphonebookaccess.h"

#include "obexphonebookaccess_p.h"
#include "pendingcall.h"

namespace BluezQt {

ObexPhonebookAccess::ObexPhonebookAccess(const QString &path, const QVariantMap &properties, QObject *parent)
    : QObject(parent)
    , d(new ObexPhonebookAccessPrivate(path, properties, this))
{

}

ObexPhonebookAccess::~ObexPhonebookAccess()
{
    delete d;
}

ObexPhonebookAccessPtr ObexPhonebookAccess::toSharedPtr() const
{
    return d->q.toStrongRef();
}

QString ObexPhonebookAccess::ubi() const
{
    return d->m_bluezPhonebookAccess->path();
}

QString ObexPhonebookAccess::folder() const
{
    return d->m_folder;
}

QString ObexPhonebookAccess::databaseIdentifier() const
{
    return d->m_databaseIdentifier;
}

QString ObexPhonebookAccess::primaryCounter() const
{
    return d->m_primaryCounter;
}

QString ObexPhonebookAccess::secondaryCounter() const
{
    return d->m_secondaryCounter;
}

bool ObexPhonebookAccess::fixedImageSize()
{
    return d->m_fixedImageSize;
}

PendingCall* ObexPhonebookAccess::select(const QString &location, const QString &phonebook)
{
    return new PendingCall(d->m_bluezPhonebookAccess->Select(location, phonebook), PendingCall::ReturnVoid, this);
}

PendingCall* ObexPhonebookAccess::pullAll(const QString &targetFile, const QVariantMap &filters)
{
    return new PendingCall(d->m_bluezPhonebookAccess->PullAll(targetFile, filters), PendingCall::ReturnTransferWithProperties, this);
}

PendingCall* ObexPhonebookAccess::pull(const QString &vcard, const QString &targetfile, const QVariantMap &filters)
{
    return new PendingCall(d->m_bluezPhonebookAccess->Pull(vcard, targetfile, filters), PendingCall::ReturnTransferWithProperties, this);
}

PendingCall* ObexPhonebookAccess::list(const QVariantMap &filters)
{
    return new PendingCall(d->m_bluezPhonebookAccess->List(filters), PendingCall::ReturnVariantMap, this);
}

PendingCall* ObexPhonebookAccess::search(const QString &field, const QString &value, const QVariantMap &filters)
{
    return new PendingCall(d->m_bluezPhonebookAccess->Search(field, value, filters), PendingCall::ReturnVariantMap, this);
}

PendingCall* ObexPhonebookAccess::getSize()
{
    return new PendingCall(d->m_bluezPhonebookAccess->GetSize(), PendingCall::ReturnUShort, this);
}

PendingCall* ObexPhonebookAccess::listFilterFields()
{
    return new PendingCall(d->m_bluezPhonebookAccess->ListFilterFields(), PendingCall::ReturnStringList, this);
}

PendingCall* ObexPhonebookAccess::updateVersion()
{
    return new PendingCall(d->m_bluezPhonebookAccess->UpdateVersion(), PendingCall::ReturnVoid, this);
}

}

