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

#ifndef OBJECT_H
#define OBJECT_H

#include <QVariant>
#include <QDBusObjectPath>

class Object
{
public:
    explicit Object();

    QObject *objectParent() const;
    void setObjectParent(QObject *parent);

    QDBusObjectPath path() const;
    void setPath(const QDBusObjectPath &path);

    QString name() const;
    void setName(const QString &name);

    bool haveProperties() const;

    QVariantMap properties() const;
    void setProperties(const QVariantMap &properties);

    QVariant property(const QString &name) const;
    void changeProperty(const QString &name, const QVariant &value);

private:
    QObject *m_parent;
    QDBusObjectPath m_path;
    QString m_name;
    QVariantMap m_properties;
};

#endif // OBJECT_H
