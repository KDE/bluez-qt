/*
 * Copyright (C) 2011 Rafael Fernández López <ereslibre@kde.org>
 * Copyright (C) 2011 UFO Coders <info@ufocoders.com>
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

#ifndef ADAPTERSRECEIVER_H
#define ADAPTERSRECEIVER_H

#include <QObject>

#include "manager.h"

class AdaptersReceiver : public QObject
{
    Q_OBJECT

public:
    explicit AdaptersReceiver(BluezQt::Manager *manager, QObject *parent = Q_NULLPTR);

public Q_SLOTS:
    void adapterAdded(const BluezQt::AdapterPtr &adapter);
    void adapterRemoved(const BluezQt::AdapterPtr &adapter);
    void usableAdapterChanged(const BluezQt::AdapterPtr &adapter);
    void allAdaptersRemoved();
    void bluetoothOperationalChanged(bool operational);
    void bluetoothBlockedChanged(bool blocked);
    void printStatus();

private:
    BluezQt::Manager *m_manager;
};

#endif // ADAPTERSRECEIVER_H
