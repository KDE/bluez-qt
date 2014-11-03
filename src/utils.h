/*****************************************************************************
 * This file is part of the QBluez project                                   *
 *                                                                           *
 * Copyright (C) 2010 Rafael Fernández López <ereslibre@kde.org>             *
 * Copyright (C) 2010 UFO Coders <info@ufocoders.com>                        *
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

#ifndef QBLUEZ_UTILS_H
#define QBLUEZ_UTILS_H

#include <QString>

namespace QBluez
{

namespace Services
{

const QString SerialPort = QLatin1String("00001101-0000-1000-8000-00805F9B34FB");
const QString DialupNetworking = QLatin1String("00001103-0000-1000-8000-00805F9B34FB");
const QString ObexObjectPush = QLatin1String("00001105-0000-1000-8000-00805F9B34FB");
const QString ObexFileTransfer = QLatin1String("00001106-0000-1000-8000-00805F9B34FB");
const QString AudioSource = QLatin1String("0000110A-0000-1000-8000-00805F9B34FB");
const QString AudioVideoRemoteControlTarget = QLatin1String("0000110C-0000-1000-8000-00805F9B34FB");
const QString AdvancedAudioDistributionProtocol = QLatin1String("0000110D-0000-1000-8000-00805F9B34FB");
const QString AudioVideoRemoteControl = QLatin1String("0000110E-0000-1000-8000-00805F9B34FB");
const QString HeadsetAudioGateway = QLatin1String("00001112-0000-1000-8000-00805F9B34FB");
const QString Nap = QLatin1String("00001116-0000-1000-8000-00805F9B34FB");
const QString HandsfreeAudioGateway = QLatin1String("0000111F-0000-1000-8000-00805F9B34FB");
const QString SimAccess = QLatin1String("0000112D-0000-1000-8000-00805F9B34FB");
const QString PhonebookAccessPse = QLatin1String("0000112F-0000-1000-8000-00805F9B34FB");
const QString MessageAccessServer = QLatin1String("00001132-0000-1000-8000-00805F9B34FB");
const QString PnpInformation = QLatin1String("00001200-0000-1000-8000-00805F9B34FB");

}

} // namespace QBluez

#endif // QBLUEZ_UTILS_H
