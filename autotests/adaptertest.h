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

#ifndef ADAPTERTEST_H
#define ADAPTERTEST_H

#include <QObject>
#include <QList>

#include "bluezadapter1_tst.h"
#include "dbusproperties_tst.h"

#include "manager.h"
#include "adapter.h"

class AdapterTest : public QObject
{
    Q_OBJECT

public:
    explicit AdapterTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void getPropertiesTest();
    void setAliasTest();
    void setPoweredTest();
    void setDiscoverableTest();
    void setDiscoverableTimeoutTest();
    void setPairableTest();
    void setPairableTimeoutTest();

    void discoveryTest();
    void removeDeviceTest();
    void adapterRemovedTest();

private:
    struct AdapterUnit
    {
        BluezQt::AdapterPtr adapter;
        org::bluez::Adapter1 *dbusAdapter;
        org::freedesktop::DBus::Properties *dbusProperties;
    };

    BluezQt::Manager *m_manager;
    QList<AdapterUnit> m_units;
};

#endif // ADAPTERTEST_H
