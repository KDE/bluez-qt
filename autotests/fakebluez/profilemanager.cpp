#include "profilemanager.h"

#include <QDebug>
#include <QDBusMessage>
#include <QDBusConnection>

ProfileManager::ProfileManager(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    setName(QStringLiteral("org.bluez.ProfileManager1"));
    setPath(QDBusObjectPath(QStringLiteral("/org/bluez")));
}

void ProfileManager::RegisterProfile(const QDBusObjectPath &path, const QString &uuid, const QVariantMap &options, const QDBusMessage &msg)
{
    Q_UNUSED(path)
    Q_UNUSED(uuid)
    Q_UNUSED(options)
    Q_UNUSED(msg)
}

void ProfileManager::UnregisterProfile(const QDBusObjectPath &path, const QDBusMessage &msg)
{
    Q_UNUSED(path)
    Q_UNUSED(msg)
}
