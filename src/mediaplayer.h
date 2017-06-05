/*
 * BluezQt - Asynchronous BlueZ wrapper library
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

#ifndef BLUEZQT_MEDIAPLAYER_H
#define BLUEZQT_MEDIAPLAYER_H

#include <QObject>

#include "types.h"
#include "mediaplayertrack.h"
#include "bluezqt_export.h"

namespace BluezQt
{

class PendingCall;

/**
 * @class BluezQt::MediaPlayer mediaplayer.h <BluezQt/MediaPlayer>
 *
 * Media player.
 *
 * This class represents a media player interface.
 */
class BLUEZQT_EXPORT MediaPlayer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(Equalizer equalizer READ equalizer WRITE setEqualizer NOTIFY equalizerChanged)
    Q_PROPERTY(Repeat repeat READ repeat WRITE setRepeat NOTIFY repeatChanged)
    Q_PROPERTY(Shuffle shuffle READ shuffle WRITE setShuffle NOTIFY shuffleChanged)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(MediaPlayerTrack track READ track NOTIFY trackChanged)
    Q_PROPERTY(quint32 position READ position NOTIFY positionChanged)

public:
    /** Equalizer state. */
    enum Equalizer {
        /** Equalizer on. */
        EqualizerOn,
        /** Equalizer off. */
        EqualizerOff
    };
    Q_ENUM(Equalizer)

    /** Repeat state. */
    enum Repeat {
        /** Repeat off. */
        RepeatOff,
        /** Repeat single track. */
        RepeatSingleTrack,
        /** Repeat all tracks. */
        RepeatAllTracks,
        /** Repeat group. */
        RepeatGroup
    };
    Q_ENUM(Repeat)

    /** Shuffle state. */
    enum Shuffle {
        /** Shuffle off. */
        ShuffleOff,
        /** Shuffle all tracks. */
        ShuffleAllTracks,
        /** Shuffle group. */
        ShuffleGroup
    };
    Q_ENUM(Shuffle)

    /** Player status. */
    enum Status {
        /** Player is playing. */
        Playing,
        /** Player is stopped. */
        Stopped,
        /** Player is paused. */
        Paused,
        /** Player is forward seeking. */
        ForwardSeek,
        /** Player is reverse seeking. */
        ReverseSeek,
        /** Error */
        Error
    };
    Q_ENUM(Status)

    /**
     * Destroys a MediaPlayer object.
     */
    ~MediaPlayer();

    /**
     * Returns a shared pointer from this.
     *
     * @return MediaPlayerPtr
     */
    MediaPlayerPtr toSharedPtr() const;

    /**
     * Returns the name of the player.
     *
     * @return name of player
     */
    QString name() const;

    /**
     * Returns the equalizer state of the player.
     *
     * @return equalizer state of player
     */
    Equalizer equalizer() const;

    /**
     * Sets the equalizer state of the player.
     *
     * @param equalizer equalizer state
     * @return void pending call
     */
    PendingCall *setEqualizer(Equalizer equalizer);

    /**
     * Returns the repeat state of the player.
     *
     * @return repeat state of player
     */
    Repeat repeat() const;

    /**
     * Sets the repeat state of the player.
     *
     * @param repeat repeat state
     * @return void pending call
     */
    PendingCall *setRepeat(Repeat repeat);

    /**
     * Returns the shuffle state of the player.
     *
     * @return shuffle state of player
     */
    Shuffle shuffle() const;

    /**
     * Sets the shuffle state of the player.
     *
     * @param shuffle shuffle state
     * @return void pending call
     */
    PendingCall *setShuffle(Shuffle shuffle);

    /**
     * Returns the status of the player.
     *
     * @return status of player
     */
    Status status() const;

    /**
     * Returns the current track.
     *
     * @return current track
     */
    MediaPlayerTrack track() const;

    /**
     * Returns the playback position in miliseconds.
     *
     * @return playback position
     */
    quint32 position() const;

public Q_SLOTS:
    /**
     * Resumes playback.
     *
     * Possible errors: PendingCall::NotSupported, PendingCall::Failed
     *
     * @return void pending call
     */
    PendingCall *play();

    /**
     * Pauses playback.
     *
     * Possible errors: PendingCall::NotSupported, PendingCall::Failed
     *
     * @return void pending call
     */
    PendingCall *pause();

    /**
     * Stops playback.
     *
     * Possible errors: PendingCall::NotSupported, PendingCall::Failed
     *
     * @return void pending call
     */
    PendingCall *stop();

    /**
     * Switch to next track.
     *
     * Possible errors: PendingCall::NotSupported, PendingCall::Failed
     *
     * @return void pending call
     */
    PendingCall *next();

    /**
     * Switch to previous track.
     *
     * Possible errors: PendingCall::NotSupported, PendingCall::Failed
     *
     * @return void pending call
     */
    PendingCall *previous();

    /**
     * Fast forwards playback.
     *
     * Possible errors: PendingCall::NotSupported, PendingCall::Failed
     *
     * @return void pending call
     */
    PendingCall *fastForward();

    /**
     * Rewinds playback.
     *
     * Possible errors: PendingCall::NotSupported, PendingCall::Failed
     *
     * @return void pending call
     */
    PendingCall *rewind();

Q_SIGNALS:
    /**
     * Indicates that player's name have changed.
     */
    void nameChanged(const QString &name);

    /**
     * Indicates that player's equalizer state have changed.
     */
    void equalizerChanged(Equalizer equalizer);

    /**
     * Indicates that player's repeat state have changed.
     */
    void repeatChanged(Repeat repeat);

    /**
     * Indicates that player's shuffle state have changed.
     */
    void shuffleChanged(Shuffle shuffle);

    /**
     * Indicates that player's status have changed.
     */
    void statusChanged(Status status);

    /**
     * Indicates that player's current track have changed.
     */
    void trackChanged(MediaPlayerTrack track);

    /**
     * Indicates that player's playback position have changed.
     */
    void positionChanged(quint32 position);

private:
    explicit MediaPlayer(const QString &path, const QVariantMap &properties);

    class MediaPlayerPrivate *const d;

    friend class MediaPlayerPrivate;
    friend class DevicePrivate;
};

} // namespace BluezQt

#endif // BLUEZQT_MEDIAPLAYER_H
