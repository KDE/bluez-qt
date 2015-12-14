/*
 * Copyright (C) 2010 Rafael Fernández López <ereslibre@kde.org>
 * Copyright (C) 2010 UFO Coders <info@ufocoders.com>
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

#ifndef DEVICERECEIVERTEST_H
#define DEVICERECEIVERTEST_H

#include <QObject>

#include "manager.h"

class DeviceReceiver : public QObject
{
    Q_OBJECT

public:
    explicit DeviceReceiver(QObject *parent = Q_NULLPTR);

    void scanDevices();

public Q_SLOTS:
    void deviceFound(const BluezQt::DevicePtr &device);
    void devicePropertyChanged(const BluezQt::DevicePtr &device);
    void adapterAdded(const BluezQt::AdapterPtr &adapter);

};

#endif // DEVICERECEIVERTEST_H
