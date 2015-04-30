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

#ifndef DEVICETEST_H
#define DEVICETEST_H

#include <QObject>
#include <QList>

#include "bluezdevice1_tst.h"
#include "dbusproperties_tst.h"

#include "manager.h"
#include "adapter.h"
#include "device.h"

class DeviceTest : public QObject
{
    Q_OBJECT

public:
    explicit DeviceTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void getPropertiesTest();
    void setAliasTest();
    void setTrustedTest();
    void setBlockedTest();

    void deviceRemovedTest();

private:
    struct DeviceUnit
    {
        BluezQt::DevicePtr device;
        org::bluez::Device1 *dbusDevice;
        org::freedesktop::DBus::Properties *dbusProperties;
    };

    QString deviceIcon(org::bluez::Device1 *device) const;
    qint16 deviceRssi(org::bluez::Device1 *device) const;

    BluezQt::Manager *m_manager;
    QList<DeviceUnit> m_units;
};

#endif // DEVICETEST_H
