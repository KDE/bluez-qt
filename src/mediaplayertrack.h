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

#ifndef BLUEZQT_MEDIAPLAYERTRACK_H
#define BLUEZQT_MEDIAPLAYERTRACK_H

#include <QString>
#include <QSharedPointer>

#include "bluezqt_export.h"

namespace BluezQt
{

/**
 * @class BluezQt::MediaPlayerTrack mediaplayertrack.h <BluezQt/MediaPlayerTrack>
 *
 * Media player track.
 *
 * This class represents a track in media player.
 */
class BLUEZQT_EXPORT MediaPlayerTrack
{
public:
    /**
     * Creates a new invalid MediaPlayerTrack object.
     */
    explicit MediaPlayerTrack();

    /**
     * Destroys a MediaPlayerTrack object.
     */
    virtual ~MediaPlayerTrack();

    /**
     * Copy constructor.
     *
     * @param other
     */
    MediaPlayerTrack(const MediaPlayerTrack &other);

    /**
     * Copy assignment operator.
     *
     * @param other
     */
    MediaPlayerTrack &operator=(const MediaPlayerTrack &other);

    /**
     * Returns whether the track is valid.
     *
     * @return true if track is valid
     */
    bool isValid() const;

    /**
     * Returns a title of the track.
     *
     * @return track title
     */
    QString title() const;

    /**
     * Returns an artist of the track.
     *
     * @return track artist
     */
    QString artist() const;

    /**
     * Returns an album of the track.
     *
     * @return track album
     */
    QString album() const;

    /**
     * Returns a genre of the track.
     *
     * @return track genre
     */
    QString genre() const;

    /**
     * Returns a total number of tracks.
     *
     * @return total number of tracks
     */
    quint32 numberOfTracks() const;

    /**
     * Returns a track number of the track.
     *
     * @return track number
     */
    quint32 trackNumber() const;

    /**
     * Returns a duration of the track.
     *
     * @return track duration
     */
    quint32 duration() const;

private:
    explicit MediaPlayerTrack(const QVariantMap &properties);

    QSharedPointer<class MediaPlayerTrackPrivate> d;

    friend class MediaPlayerPrivate;
};

} // namespace BluezQt

Q_DECLARE_METATYPE(BluezQt::MediaPlayerTrack)

#endif // BLUEZQT_MEDIAPLAYERTRACK_H
