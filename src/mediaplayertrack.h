/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * SPDX-FileCopyrightText: 2015 David Rosca <nowrep@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef BLUEZQT_MEDIAPLAYERTRACK_H
#define BLUEZQT_MEDIAPLAYERTRACK_H

#include <QSharedPointer>
#include <QString>

#include "bluezqt_export.h"

namespace BluezQt
{
/*!
 * \inmodule BluezQt
 * \class BluezQt::MediaPlayerTrack
 * \inheaderfile BluezQt/MediaPlayerTrack
 * \brief Media player track.
 *
 * This class represents a track in media player.
 */
class BLUEZQT_EXPORT MediaPlayerTrack
{
public:
    /*!
     * Creates a new invalid MediaPlayerTrack object.
     */
    explicit MediaPlayerTrack();

    virtual ~MediaPlayerTrack();

    /*!
     * Constructs a new MediaPlayerTrack object from \a other.
     */
    MediaPlayerTrack(const MediaPlayerTrack &other);

    /*!
     * Copies the MediaPlayerTrack object from \a other.
     */
    MediaPlayerTrack &operator=(const MediaPlayerTrack &other);

    /*!
     * Returns whether the track is valid.
     */
    bool isValid() const;

    /*!
     * Returns the track title.
     */
    QString title() const;

    /*!
     * Returns the track artist.
     */
    QString artist() const;

    /*!
     * Returns the track album.
     */
    QString album() const;

    /*!
     * Returns the track genre.
     */
    QString genre() const;

    /*!
     * Returns the total number of tracks.
     */
    quint32 numberOfTracks() const;

    /*!
     * Returns the track number.
     */
    quint32 trackNumber() const;

    /*!
     * Returns the track duration.
     */
    quint32 duration() const;

private:
    BLUEZQT_NO_EXPORT explicit MediaPlayerTrack(const QVariantMap &properties);

    QSharedPointer<class MediaPlayerTrackPrivate> d;

    friend class MediaPlayerPrivate;
};

} // namespace BluezQt

Q_DECLARE_METATYPE(BluezQt::MediaPlayerTrack)

#endif // BLUEZQT_MEDIAPLAYERTRACK_H
