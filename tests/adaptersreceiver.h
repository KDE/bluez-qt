/*****************************************************************************
 * This file is part of the BluezQt project                                   *
 *                                                                           *
 * Copyright (C) 2011 Rafael Fernández López <ereslibre@kde.org>             *
 * Copyright (C) 2011 UFO Coders <info@ufocoders.com>                        *
 * Copyright (C) 2014 David Rosca <nowrep@gmail.com>                         *
 *                                                                           *
 * This library is free software; you can redistribute it and/or             *
 * modify it under the terms of the GNU Library General Public               *
 * License as published by the Free Software Foundation; either              *
 * version 2 of the License, or (at your option) any later version.          *
 *                                                                           *
 * This library is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         *
 * Library General Public License for more details.                          *
 *                                                                           *
 * You should have received a copy of the GNU Library General Public License *
 * along with this library; see the file COPYING.LIB.  If not, write to      *
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,      *
 * Boston, MA 02110-1301, USA.                                               *
 *****************************************************************************/

#ifndef ADAPTERSRECEIVER_H
#define ADAPTERSRECEIVER_H

#include <QObject>

#include "manager.h"

class AdaptersReceiver : public QObject
{
    Q_OBJECT

public:
    AdaptersReceiver(BluezQt::Manager *manager, QObject *parent = Q_NULLPTR);

public Q_SLOTS:
    void adapterAdded(BluezQt::AdapterPtr adapter);
    void adapterRemoved(BluezQt::AdapterPtr adapter);
    void usableAdapterChanged(BluezQt::AdapterPtr adapter);
    void allAdaptersRemoved();
    void bluetoothOperationalChanged(bool operational);
    void bluetoothBlockedChanged(bool blocked);
    void printStatus();

private:
    BluezQt::Manager *m_manager;
};

#endif // ADAPTERSRECEIVER_H
