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

#pragma once

#include <QObject>
#include <QList>

#include "bluezdevice1_tst.h"
#include "bluezmediatransport1_tst.h"
#include "dbusproperties_tst.h"

#include "manager.h"
#include "device.h"
#include "mediatransport.h"

class MediaTransportTest : public QObject
{
    Q_OBJECT

public:
    explicit MediaTransportTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void connectTest();
    void disconnectTest();
    void connectProfileTest();

    void getPropertiesTest();

    void disconnectProfileTest();

private:
    struct MediaTransportUnit
    {
        BluezQt::DevicePtr device;
        org::bluez::MediaTransport1 *dbusMediaTransport;
        org::freedesktop::DBus::Properties *dbusProperties;
    };

    BluezQt::Manager *m_manager;
    QList<MediaTransportUnit> m_units;
};
