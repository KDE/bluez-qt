#include "objectmanager.h"
#include "object.h"

#include <QDebug>
#include <QDBusMetaType>
#include <QDBusConnection>

ObjectManager *s_instance = 0;

ObjectManager::ObjectManager(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    s_instance = this;

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
    m_objects.insert(object->path().path(), object);

    QVariantMapMap interfaces;
    interfaces.insert(object->name(), object->properties());
    Q_EMIT InterfacesAdded(object->path(), interfaces);
}

void ObjectManager::removeObject(Object *object)
{
    m_objects.remove(object->path().path());

    Q_EMIT InterfacesRemoved(object->path(), QStringList(object->name()));
}

void ObjectManager::addAutoDeleteObject(QObject *object)
{
    m_autoDeleteObjects.append(object);
}

Object *ObjectManager::objectByPath(const QDBusObjectPath &path) const
{
    return m_objects.value(path.path());
}

ObjectManager *ObjectManager::self()
{
    return s_instance;
}

DBusManagerStruct ObjectManager::GetManagedObjects()
{
    DBusManagerStruct objects;

    Q_FOREACH (Object *object, m_objects.values()) {
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
