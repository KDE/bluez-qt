#include "obexphonebookaccess_p.h"

#include "obexphonebookaccess1.h"
#include "utils.h"

namespace BluezQt
{

ObexPhonebookAccessPrivate::ObexPhonebookAccessPrivate(const QString &path, const QVariantMap &properties, QObject *parent)
    : QObject(parent)
    , m_bluezPhonebookAccess(nullptr)
{
    m_bluezPhonebookAccess = new BluezPhonebookAccess(Strings::orgBluezObex(), path, DBusConnection::orgBluezObex(), this);

    init(properties);
}

void ObexPhonebookAccessPrivate::init(const QVariantMap &properties)
{
    m_folder = properties.value(QStringLiteral("Folder")).toString();
    m_databaseIdentifier = properties.value(QStringLiteral("DatabaseIdentifier")).toString();
    m_primaryCounter = properties.value(QStringLiteral("PrimaryCounter")).toString();
    m_secondaryCounter = properties.value(QStringLiteral("SecondaryCounter")).toString();
    m_fixedImageSize = properties.value(QStringLiteral("FixedImageSize")).toBool();
}

}
