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

ObjectManager::~ObjectManager()
{
    qDeleteAll(m_autoDeleteObjects);
}

void ObjectManager::addObject(Object *object)
{
    m_objects.append(object);

    QVariantMapMap interfaces;
    interfaces.insert(object->name(), object->properties());
    Q_EMIT InterfacesAdded(object->path(), interfaces);
}

void ObjectManager::removeObject(Object *object)
{
    m_objects.removeOne(object);

    Q_EMIT InterfacesRemoved(object->path(), QStringList(object->name()));
}

void ObjectManager::addAutoDeleteObject(QObject *object)
{
    m_autoDeleteObjects.append(object);
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
