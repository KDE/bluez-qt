/*****************************************************************************
 * This file is part of the QBluez project                                   *
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
#include <QThread>

namespace QBluez {
    class Manager;
    class Adapter;
    class Device;
}

class AdaptersReceiver : public QThread
{
    Q_OBJECT

public:
    AdaptersReceiver(QBluez::Manager *manager, QObject *parent = 0);
    virtual ~AdaptersReceiver();

public Q_SLOTS:
    void adapterAdded(QBluez::Adapter *adapter);
    void adapterRemoved(QBluez::Adapter *adapter);
    void usableAdapterChanged(QBluez::Adapter *adapter);
    void allAdaptersRemoved();

protected:
    virtual void run();

private:
    QBluez::Manager *m_manager;
};

#endif // ADAPTERSRECEIVER_H
