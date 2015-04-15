/*
 * BluezQt - Asynchronous Bluez wrapper library
 *
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

#ifndef DECLARATIVEMEDIAPLAYER_H
#define DECLARATIVEMEDIAPLAYER_H

#include <QJsonObject>

#include "mediaplayer.h"

class DeclarativeMediaPlayer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(BluezQt::MediaPlayer::Equalizer equalizer READ equalizer WRITE setEqualizer NOTIFY equalizerChanged)
    Q_PROPERTY(BluezQt::MediaPlayer::Repeat repeat READ repeat WRITE setRepeat NOTIFY repeatChanged)
    Q_PROPERTY(BluezQt::MediaPlayer::Shuffle shuffle READ shuffle WRITE setShuffle NOTIFY shuffleChanged)
    Q_PROPERTY(BluezQt::MediaPlayer::Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(QJsonObject track READ track NOTIFY trackChanged)
    Q_PROPERTY(quint32 position READ position NOTIFY positionChanged)

public:
    explicit DeclarativeMediaPlayer(BluezQt::MediaPlayerPtr mediaPlayer, QObject *parent = Q_NULLPTR);

    QString name() const;

    BluezQt::MediaPlayer::Equalizer equalizer() const;
    void setEqualizer(BluezQt::MediaPlayer::Equalizer equalizer);

    BluezQt::MediaPlayer::Repeat repeat() const;
    void setRepeat(BluezQt::MediaPlayer::Repeat repeat);

    BluezQt::MediaPlayer::Shuffle shuffle() const;
    void setShuffle(BluezQt::MediaPlayer::Shuffle shuffle);

    BluezQt::MediaPlayer::Status status() const;

    QJsonObject track() const;

    quint32 position() const;

public Q_SLOTS:
    BluezQt::PendingCall *play();
    BluezQt::PendingCall *pause();
    BluezQt::PendingCall *stop();
    BluezQt::PendingCall *next();
    BluezQt::PendingCall *previous();
    BluezQt::PendingCall *fastForward();
    BluezQt::PendingCall *rewind();

Q_SIGNALS:
    void nameChanged(const QString &name);
    void equalizerChanged(BluezQt::MediaPlayer::Equalizer equalizer);
    void repeatChanged(BluezQt::MediaPlayer::Repeat repeat);
    void shuffleChanged(BluezQt::MediaPlayer::Shuffle shuffle);
    void statusChanged(BluezQt::MediaPlayer::Status status);
    void trackChanged(const QJsonObject &track);
    void positionChanged(quint32 position);

private:
    void updateTrack();

    BluezQt::MediaPlayerPtr m_mediaPlayer;
    QJsonObject m_track;
};

#endif // DECLARATIVEMEDIAPLAYER_H
