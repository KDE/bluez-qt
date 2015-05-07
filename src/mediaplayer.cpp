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

#include "mediaplayer.h"
#include "mediaplayer_p.h"
#include "pendingcall.h"

namespace BluezQt
{

static QString equalizerToString(MediaPlayer::Equalizer equalizer)
{
    switch (equalizer) {
    case MediaPlayer::EqualizerOn:
        return QStringLiteral("on");
    default:
        return QStringLiteral("off");
    }
}

static QString repeatToString(MediaPlayer::Repeat repeat)
{
    switch (repeat) {
    case MediaPlayer::RepeatSingleTrack:
        return QStringLiteral("singletrack");
    case MediaPlayer::RepeatAllTracks:
        return QStringLiteral("alltracks");
    case MediaPlayer::RepeatGroup:
        return QStringLiteral("group");
    default:
        return QStringLiteral("off");
    }
}

static QString shuffleToString(MediaPlayer::Shuffle shuffle)
{
    switch (shuffle) {
    case MediaPlayer::ShuffleAllTracks:
        return QStringLiteral("alltracks");
    case MediaPlayer::ShuffleGroup:
        return QStringLiteral("group");
    default:
        return QStringLiteral("off");
    }
}

MediaPlayer::MediaPlayer(const QString &path, const QVariantMap &properties)
    : QObject()
    , d(new MediaPlayerPrivate(path, properties))
{
}

MediaPlayer::~MediaPlayer()
{
    delete d;
}

MediaPlayerPtr MediaPlayer::toSharedPtr() const
{
    return d->q.toStrongRef();
}

QString MediaPlayer::name() const
{
    return d->m_name;
}

MediaPlayer::Equalizer MediaPlayer::equalizer() const
{
    return d->m_equalizer;
}

PendingCall *MediaPlayer::setEqualizer(MediaPlayer::Equalizer equalizer)
{
    return new PendingCall(d->setDBusProperty(QStringLiteral("Equalizer"), equalizerToString(equalizer)),
                           PendingCall::ReturnVoid, this);
}

MediaPlayer::Repeat MediaPlayer::repeat() const
{
    return d->m_repeat;
}

PendingCall *MediaPlayer::setRepeat(MediaPlayer::Repeat repeat)
{
    return new PendingCall(d->setDBusProperty(QStringLiteral("Repeat"), repeatToString(repeat)),
                           PendingCall::ReturnVoid, this);
}

MediaPlayer::Shuffle MediaPlayer::shuffle() const
{
    return d->m_shuffle;
}

PendingCall *MediaPlayer::setShuffle(MediaPlayer::Shuffle shuffle)
{
    return new PendingCall(d->setDBusProperty(QStringLiteral("Shuffle"), shuffleToString(shuffle)),
                           PendingCall::ReturnVoid, this);
}

MediaPlayer::Status MediaPlayer::status() const
{
    return d->m_status;
}

MediaPlayerTrack MediaPlayer::track() const
{
    return d->m_track;
}

quint32 MediaPlayer::position() const
{
    return d->m_position;
}

PendingCall *MediaPlayer::play()
{
    return new PendingCall(d->m_bluezMediaPlayer->Play(), PendingCall::ReturnVoid, this);
}

PendingCall *MediaPlayer::pause()
{
    return new PendingCall(d->m_bluezMediaPlayer->Pause(), PendingCall::ReturnVoid, this);
}

PendingCall *MediaPlayer::stop()
{
    return new PendingCall(d->m_bluezMediaPlayer->Stop(), PendingCall::ReturnVoid, this);
}

PendingCall *MediaPlayer::next()
{
    return new PendingCall(d->m_bluezMediaPlayer->Next(), PendingCall::ReturnVoid, this);
}

PendingCall *MediaPlayer::previous()
{
    return new PendingCall(d->m_bluezMediaPlayer->Previous(), PendingCall::ReturnVoid, this);
}

PendingCall *MediaPlayer::fastForward()
{
    return new PendingCall(d->m_bluezMediaPlayer->FastForward(), PendingCall::ReturnVoid, this);
}

PendingCall *MediaPlayer::rewind()
{
    return new PendingCall(d->m_bluezMediaPlayer->Rewind(), PendingCall::ReturnVoid, this);
}

} // namespace BluezQt
