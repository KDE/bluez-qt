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

#include "object.h"

#include <QDBusMessage>
#include <QDBusConnection>
#include <QDBusAbstractAdaptor>

Object::Object()
    : m_parent(0)
{
}

QObject *Object::objectParent() const
{
    return m_parent;
}

void Object::setObjectParent(QObject *parent)
{
    m_parent = parent;
}

QDBusObjectPath Object::path() const
{
    return m_path;
}

void Object::setPath(const QDBusObjectPath &path)
{
    m_path = path;
}

QString Object::name() const
{
    return m_name;
}

void Object::setName(const QString &name)
{
    m_name = name;
}

bool Object::haveProperties() const
{
    return !m_properties.isEmpty();
}

QVariantMap Object::properties() const
{
    return m_properties;
}

void Object::setProperties(const QVariantMap &properties)
{
    m_properties = properties;
}

QVariant Object::property(const QString &name) const
{
    return m_properties.value(name);
}

void Object::changeProperty(const QString &name, const QVariant &value)
{
    if (m_properties.value(name) == value) {
        return;
    }

    QVariantMap updatedProperties;
    updatedProperties[name] = value;

    m_properties[name] = value;

    QDBusMessage signal = QDBusMessage::createSignal(m_path.path(),
                                                     QStringLiteral("org.freedesktop.DBus.Properties"),
                                                     QStringLiteral("PropertiesChanged"));
    signal << m_name;
    signal << updatedProperties;
    signal << QStringList();
    QDBusConnection::sessionBus().send(signal);
}
