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

#ifndef INPUTTEST_H
#define INPUTTEST_H

#include <QObject>

#include "bluezinput1_tst.h"

#include "manager.h"
#include "device.h"
#include "input.h"

class InputTest : public QObject
{
    Q_OBJECT

public:
    explicit InputTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void getPropertiesTest();

private:
    QString reconnectModeString(const BluezQt::InputPtr &input) const;

    struct InputUnit
    {
        BluezQt::DevicePtr device;
        org::bluez::Input1 *dbusInput;
    };

    BluezQt::Manager *m_manager;
    QList<InputUnit> m_units;
};

#endif // INPUTTEST_H
