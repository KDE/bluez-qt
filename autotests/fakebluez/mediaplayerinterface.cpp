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

#include "mediaplayerinterface.h"
#include "objectmanager.h"

#include <QDBusMessage>
#include <QDBusArgument>
#include <QDBusConnection>

// MediaPlayerObject
MediaPlayerObject::MediaPlayerObject(const QDBusObjectPath &path, QObject *parent)
    : QObject(parent)
{
    QDBusConnection::sessionBus().registerObject(path.path(), this);
}

// MediaPlayerInterface
MediaPlayerInterface::MediaPlayerInterface(const QDBusObjectPath &path, const QVariantMap &properties, QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    setPath(path);
    setObjectParent(parent);
    setProperties(properties);
    setName(QStringLiteral("org.bluez.MediaPlayer1"));
}

QString MediaPlayerInterface::name() const
{
    return Object::property(QStringLiteral("Name")).toString();
}

QString MediaPlayerInterface::equalizer() const
{
    return Object::property(QStringLiteral("Equalizer")).toString();
}

void MediaPlayerInterface::setEqualizer(const QString &equalizer)
{
    Object::changeProperty(QStringLiteral("Equalizer"), equalizer);
}

QString MediaPlayerInterface::repeat() const
{
    return Object::property(QStringLiteral("Repeat")).toString();
}

void MediaPlayerInterface::setRepeat(const QString &repeat)
{
    Object::changeProperty(QStringLiteral("Repeat"), repeat);
}

QString MediaPlayerInterface::shuffle() const
{
    return Object::property(QStringLiteral("Shuffle")).toString();
}

void MediaPlayerInterface::setShuffle(const QString &shuffle)
{
    Object::changeProperty(QStringLiteral("Shuffle"), shuffle);
}

QString MediaPlayerInterface::status() const
{
    return Object::property(QStringLiteral("Status")).toString();
}

QVariantMap MediaPlayerInterface::track() const
{
    return qdbus_cast<QVariantMap>(Object::property(QStringLiteral("Track")));
}

quint32 MediaPlayerInterface::position() const
{
    return Object::property(QStringLiteral("Position")).toUInt();
}

QDBusObjectPath MediaPlayerInterface::device() const
{
    return Object::property(QStringLiteral("Device")).value<QDBusObjectPath>();
}

void MediaPlayerInterface::Play()
{
    Object::changeProperty(QStringLiteral("Status"), QStringLiteral("playing"));
}

void MediaPlayerInterface::Pause()
{
    Object::changeProperty(QStringLiteral("Status"), QStringLiteral("paused"));
}

void MediaPlayerInterface::Stop()
{
    Object::changeProperty(QStringLiteral("Status"), QStringLiteral("stopped"));
}

void MediaPlayerInterface::Next()
{
    QVariantMap t = track();
    t[QStringLiteral("Title")] = QVariant(t[QStringLiteral("Title")].toString() + QLatin1String("_next"));
    t[QStringLiteral("TrackNumber")] = t[QStringLiteral("TrackNumber")].toUInt() + 1;

    Object::changeProperty(QStringLiteral("Track"), t);
}

void MediaPlayerInterface::Previous()
{
    QVariantMap t = track();
    t[QStringLiteral("Title")] = QVariant(t[QStringLiteral("Title")].toString() + QLatin1String("_prev"));
    t[QStringLiteral("TrackNumber")] = t[QStringLiteral("TrackNumber")].toUInt() - 1;

    Object::changeProperty(QStringLiteral("Track"), t);
}

void MediaPlayerInterface::FastForward()
{
    Object::changeProperty(QStringLiteral("Status"), QStringLiteral("forward-seek"));
}

void MediaPlayerInterface::Rewind()
{
    Object::changeProperty(QStringLiteral("Status"), QStringLiteral("reverse-seek"));
}
