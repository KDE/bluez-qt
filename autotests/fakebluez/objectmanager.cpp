#include "objectmanager.h"
#include "object.h"

#include <QDebug>
#include <QDBusMetaType>
#include <QDBusConnection>

ObjectManager::ObjectManager(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    qDBusRegisterMetaType<QVariantMapMap>();
    qDBusRegisterMetaType<DBusManagerStruct>();

    QDBusConnection::sessionBus().registerObject(QStringLiteral("/org/bluez"), this);
}

void ObjectManager::addObject(Object *object)
{
    m_objects.append(object);
}

DBusManagerStruct ObjectManager::GetManagedObjects()
{
    DBusManagerStruct objects;

    Q_FOREACH (Object *object, m_objects) {
        if (objects.value(object->path()).isEmpty()) {
            objects[object->path()].insert(QStringLiteral("org.freedesktop.DBus.Introspectable"), QVariantMap());
            if (object->haveProperties()) {
                objects[object->path()].insert(QStringLiteral("org.freedesktop.DBus.Properties"), QVariantMap());
            }
        }
        objects[object->path()].insert(object->name(), object->properties());
    }

    return objects;
}
