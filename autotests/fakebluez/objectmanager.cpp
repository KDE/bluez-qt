/*
 * Copyright (C) 2014-2015 David Rosca <nowrep@gmail.com>
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

#include "objectmanager.h"
#include "object.h"

#include <QDBusMetaType>
#include <QDBusConnection>

ObjectManager *s_instance = Q_NULLPTR;

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
    s_instance = Q_NULLPTR;
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
    m_autoDeleteObjects.removeOne(object->objectParent());

    Q_EMIT InterfacesRemoved(object->path(), QStringList(object->name()));

    delete object->objectParent();
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
