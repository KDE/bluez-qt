/*
 * BluezQt - Asynchronous Bluez wrapper library
 *
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

#ifndef BLUEZQT_UTILS_H
#define BLUEZQT_UTILS_H

#include "device.h"

class QString;
class QStringList;
class QDBusConnection;

namespace BluezQt
{

namespace Strings
{

QString orgFreedesktopDBus();
QString orgBluez();
QString orgBluezAdapter1();
QString orgBluezDevice1();
QString orgBluezInput1();
QString orgBluezMediaPlayer1();
QString orgBluezAgentManager1();
QString orgBluezProfileManager1();
QString orgBluezObex();
QString orgBluezObexClient1();
QString orgBluezObexAgentManager1();
QString orgBluezObexSession1();
QString orgBluezObexTransfer1();

}

namespace DBusConnection
{

QDBusConnection orgBluez();
QDBusConnection orgBluezObex();

}

namespace Instance
{

Manager *manager();
void setManager(Manager *manager);

ObexManager *obexManager();
void setObexManager(ObexManager *obexManager);

}

QStringList stringListToUpper(const QStringList &list);
Device::Type classToType(quint32 classNum);

} // namespace BluezQt

#endif // BLUEZQT_UTILS_H
