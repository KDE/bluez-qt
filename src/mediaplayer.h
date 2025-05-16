/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * SPDX-FileCopyrightText: 2015 David Rosca <nowrep@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef BLUEZQT_MEDIAPLAYER_H
#define BLUEZQT_MEDIAPLAYER_H

#include <QObject>

#include "bluezqt_export.h"
#include "mediaplayertrack.h"
#include "types.h"

#include <memory>

namespace BluezQt
{
class PendingCall;

/*!
 * \inmodule BluezQt
 * \class BluezQt::MediaPlayer
 * \inheaderfile BluezQt/MediaPlayer
 * \brief Media player.
 *
 * This class represents a media player interface.
 */
class BLUEZQT_EXPORT MediaPlayer : public QObject
{
    Q_OBJECT
    /*! \property BluezQt::MediaPlayer::name */
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    /*! \property BluezQt::MediaPlayer::equalizer */
    Q_PROPERTY(Equalizer equalizer READ equalizer WRITE setEqualizer NOTIFY equalizerChanged)
    /*! \property BluezQt::MediaPlayer::repeat */
    Q_PROPERTY(Repeat repeat READ repeat WRITE setRepeat NOTIFY repeatChanged)
    /*! \property BluezQt::MediaPlayer::shuffle */
    Q_PROPERTY(Shuffle shuffle READ shuffle WRITE setShuffle NOTIFY shuffleChanged)
    /*! \property BluezQt::MediaPlayer::status */
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    /*! \property BluezQt::MediaPlayer::track */
    Q_PROPERTY(MediaPlayerTrack track READ track NOTIFY trackChanged)
    /*! \property BluezQt::MediaPlayer::position */
    Q_PROPERTY(quint32 position READ position NOTIFY positionChanged)

public:
    /*!
     * \value EqualizerOn
     * \value EqualizerOff
     */
    enum Equalizer {
        EqualizerOn,
        EqualizerOff,
    };
    Q_ENUM(Equalizer)

    /*!
     * \value RepeatOff
     * \value RepeatSingleTrack
     * \value RepeatAllTracks
     * \value RepeatGroup
     */
    enum Repeat {
        RepeatOff,
        RepeatSingleTrack,
        RepeatAllTracks,
        RepeatGroup,
    };
    Q_ENUM(Repeat)

    /*!
     * \value ShuffleOff
     * \value ShuffleAllTracks
     * \value ShuffleGroup
     */
    enum Shuffle {
        ShuffleOff,
        ShuffleAllTracks,
        ShuffleGroup,
    };
    Q_ENUM(Shuffle)

    /*!
     * \value Playing
     * \value Stopped
     * \value Paused
     * \value ForwardSeek
     * \value ReverseSeek
     * \value Error
     */
    enum Status {
        Playing,
        Stopped,
        Paused,
        ForwardSeek,
        ReverseSeek,
        Error,
    };
    Q_ENUM(Status)

    ~MediaPlayer() override;

    /*!
     * Returns a shared pointer from this.
     */
    MediaPlayerPtr toSharedPtr() const;

    /*!
     * Returns the name of the player.
     */
    QString name() const;

    /*!
     * Returns the equalizer state of the player.
     */
    Equalizer equalizer() const;

    /*!
     * Sets the \a equalizer state of the player.
     *
     * Returns void pending call.
     */
    PendingCall *setEqualizer(Equalizer equalizer);

    /*!
     * Returns the repeat state of the player.
     */
    Repeat repeat() const;

    /*!
     * Sets the \a repeat state of the player.
     *
     * Returns void pending call.
     */
    PendingCall *setRepeat(Repeat repeat);

    /*!
     * Returns the shuffle state of the player.
     */
    Shuffle shuffle() const;

    /*!
     * Sets the \a shuffle state of the player.
     *
     * Returns void pending call.
     */
    PendingCall *setShuffle(Shuffle shuffle);

    /*!
     * Returns the status of the player.
     */
    Status status() const;

    /*!
     * Returns the current track.
     */
    MediaPlayerTrack track() const;

    /*!
     * Returns the playback position in milliseconds.
     */
    quint32 position() const;

public Q_SLOTS:
    /*!
     * Resumes playback.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::NotSupported
     * \li PendingCall::Failed
     * \endlist
     *
     * Returns void pending call.
     */
    PendingCall *play();

    /*!
     * Pauses playback.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::NotSupported
     * \li PendingCall::Failed
     * \endlist
     *
     * Returns void pending call.
     */
    PendingCall *pause();

    /*!
     * Stops playback.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::NotSupported
     * \li PendingCall::Failed
     * \endlist
     *
     * Returns void pending call.
     */
    PendingCall *stop();

    /*!
     * Switches to the next track.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::NotSupported
     * \li PendingCall::Failed
     * \endlist
     *
     * Returns void pending call.
     */
    PendingCall *next();

    /*!
     * Switch to previous track.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::NotSupported
     * \li PendingCall::Failed
     * \endlist
     *
     * Returns void pending call.
     */
    PendingCall *previous();

    /*!
     * Fast forwards playback.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::NotSupported
     * \li PendingCall::Failed
     * \endlist
     *
     * Returns void pending call.
     */
    PendingCall *fastForward();

    /*!
     * Rewinds playback.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::NotSupported
     * \li PendingCall::Failed
     * \endlist
     *
     * Returns void pending call.
     */
    PendingCall *rewind();

Q_SIGNALS:
    /*!
     * Indicates that the player's \a name has changed.
     */
    void nameChanged(const QString &name);

    /*!
     * Indicates that the player's \a equalizer state has changed.
     */
    void equalizerChanged(Equalizer equalizer);

    /*!
     * Indicates that the player's \a repeat state has changed.
     */
    void repeatChanged(Repeat repeat);

    /*!
     * Indicates that the player's \a shuffle state has changed.
     */
    void shuffleChanged(Shuffle shuffle);

    /*!
     * Indicates that the player's \a status has changed.
     */
    void statusChanged(Status status);

    /*!
     * Indicates that the player's current \a track has changed.
     */
    void trackChanged(MediaPlayerTrack track);

    /*!
     * Indicates that the player's playback \a position has changed.
     */
    void positionChanged(quint32 position);

private:
    BLUEZQT_NO_EXPORT explicit MediaPlayer(const QString &path, const QVariantMap &properties);

    std::unique_ptr<class MediaPlayerPrivate> const d;

    friend class MediaPlayerPrivate;
    friend class DevicePrivate;
};

} // namespace BluezQt

#endif // BLUEZQT_MEDIAPLAYER_H
