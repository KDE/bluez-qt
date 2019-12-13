/*
 * Copyright (C) 2019 Kai Uwe Broulik <kde@broulik.de>
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

#ifndef BATTERYTEST_H
#define BATTERYTEST_H

#include <QObject>

#include "bluezbattery1_tst.h"

#include "manager.h"
#include "device.h"
#include "battery.h"

class BatteryTest : public QObject
{
    Q_OBJECT

public:
    explicit BatteryTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void getPropertiesTest();

private:
    struct BatteryUnit
    {
        BluezQt::DevicePtr device;
        org::bluez::Battery1 *dbusBattery;
    };

    BluezQt::Manager *m_manager;
    QList<BatteryUnit> m_units;
};

#endif // BATTERYTEST_H
