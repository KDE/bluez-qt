/*
 * Copyright (C) 2019 Manuel Weichselbaumer <mincequi@web.de>
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

#include "mediatransportinterface.h"
#include "objectmanager.h"

#include <QDBusMessage>
#include <QDBusArgument>
#include <QDBusConnection>

MediaTransportObject::MediaTransportObject(const QDBusObjectPath &path, QObject *parent)
    : QObject(parent)
{
    QDBusConnection::sessionBus().registerObject(path.path(), this);
}

MediaTransportInterface::MediaTransportInterface(const QDBusObjectPath &path, const QVariantMap &properties, QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    setPath(path);
    setObjectParent(parent);
    setProperties(properties);
    setName(QStringLiteral("org.bluez.MediaTransport1"));
}

quint8 MediaTransportInterface::codec() const
{
    return Object::property(QStringLiteral("Codec")).toUInt();
}

QByteArray MediaTransportInterface::configuration() const
{
    return Object::property(QStringLiteral("Configuration")).toByteArray();
}

QString MediaTransportInterface::state() const
{
    return Object::property(QStringLiteral("State")).toString();
}

quint16 MediaTransportInterface::volume() const
{
    return Object::property(QStringLiteral("Volume")).toUInt();
}

void MediaTransportInterface::Acquire(const QDBusMessage &msg)
{
}

void MediaTransportInterface::TryAcquire(const QDBusMessage &msg)
{
}

void MediaTransportInterface::Release()
{
}
