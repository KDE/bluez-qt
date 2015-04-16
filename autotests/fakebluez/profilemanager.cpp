/*
 * Copyright (C) 2015 David Rosca <nowrep@gmail.com>
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
