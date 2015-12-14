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

#ifndef MEDIAPLAYERTEST_H
#define MEDIAPLAYERTEST_H

#include <QObject>
#include <QList>

#include "bluezdevice1_tst.h"
#include "bluezmediaplayer1_tst.h"
#include "dbusproperties_tst.h"

#include "manager.h"
#include "device.h"
#include "mediaplayer.h"

class MediaPlayerTest : public QObject
{
    Q_OBJECT

public:
    explicit MediaPlayerTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void connectTest();
    void disconnectTest();
    void connectProfileTest();

    void getPropertiesTest();
    void setEqualizerTest();
    void setRepeatTest();
    void setShuffleTest();
    void changeStatusTest();
    void changeTrackTest();

    void disconnectProfileTest();

private:
    QString equalizerString(const BluezQt::MediaPlayerPtr &mediaPlayer) const;
    QString repeatString(const BluezQt::MediaPlayerPtr &mediaPlayer) const;
    QString shuffleString(const BluezQt::MediaPlayerPtr &mediaPlayer) const;
    QString statusString(const BluezQt::MediaPlayerPtr &mediaPlayer) const;
    QVariantMap trackMap(const BluezQt::MediaPlayerPtr &mediaPlayer) const;

    struct MediaPlayerUnit
    {
        BluezQt::DevicePtr device;
        org::bluez::MediaPlayer1 *dbusMediaPlayer;
        org::freedesktop::DBus::Properties *dbusProperties;
    };

    BluezQt::Manager *m_manager;
    QList<MediaPlayerUnit> m_units;
};

#endif // MEDIAPLAYERTEST_H
