#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

#include "object.h"

#include <QDBusAbstractAdaptor>

class QDBusMessage;

class ProfileManager : public QDBusAbstractAdaptor, public Object
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.bluez.ProfileManager1")

public:
    explicit ProfileManager(QObject *parent = 0);

public Q_SLOTS:
    void RegisterProfile(const QDBusObjectPath &path, const QString &uuid, const QVariantMap &options, const QDBusMessage &msg);
    void UnregisterProfile(const QDBusObjectPath &path, const QDBusMessage &msg);
};

#endif // PROFILEMANAGER_H
