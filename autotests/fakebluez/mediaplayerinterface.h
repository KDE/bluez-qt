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

#ifndef MEDIAPLAYERINTERFACE_H
#define MEDIAPLAYERINTERFACE_H

#include "object.h"

#include <QDBusAbstractAdaptor>

class QDBusMessage;
class QDBusObjectPath;

class MediaPlayerObject : public QObject
{
public:
    explicit MediaPlayerObject(const QDBusObjectPath &path, QObject *parent = Q_NULLPTR);
};

class MediaPlayerInterface : public QDBusAbstractAdaptor, public Object
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.bluez.MediaPlayer1")
    Q_PROPERTY(QString Name READ name)
    Q_PROPERTY(QString Equalizer READ equalizer WRITE setEqualizer)
    Q_PROPERTY(QString Repeat READ repeat WRITE setRepeat)
    Q_PROPERTY(QString Shuffle READ shuffle WRITE setShuffle)
    Q_PROPERTY(QString Status READ status)
    Q_PROPERTY(QVariantMap Track READ track)
    Q_PROPERTY(quint32 Position READ position)
    Q_PROPERTY(QDBusObjectPath Device READ device)

public:
    explicit MediaPlayerInterface(const QDBusObjectPath &path, const QVariantMap &properties, QObject *parent = Q_NULLPTR);

    QString name() const;

    QString equalizer() const;
    void setEqualizer(const QString &equalizer);

    QString repeat() const;
    void setRepeat(const QString &repeat);

    QString shuffle() const;
    void setShuffle(const QString &shuffle);

    QString status() const;

    QVariantMap track() const;

    quint32 position() const;

    QDBusObjectPath device() const;

public Q_SLOTS:
    void Play();
    void Pause();
    void Stop();
    void Next();
    void Previous();
    void FastForward();
    void Rewind();
};

#endif // MEDIAPLAYERINTERFACE_H
