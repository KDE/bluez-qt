/*
 * BluezQt - Asynchronous BlueZ wrapper library
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

#ifndef BLUEZQT_SERVICES_H
#define BLUEZQT_SERVICES_H

#include <QString>

namespace BluezQt
{

/** Service UUIDs. */
namespace Services
{

const QString ServiceDiscoveryServer = QLatin1String("00001000-0000-1000-8000-00805F9B34FB");
const QString SerialPort = QLatin1String("00001101-0000-1000-8000-00805F9B34FB");
const QString DialupNetworking = QLatin1String("00001103-0000-1000-8000-00805F9B34FB");
const QString ObexObjectPush = QLatin1String("00001105-0000-1000-8000-00805F9B34FB");
const QString ObexFileTransfer = QLatin1String("00001106-0000-1000-8000-00805F9B34FB");
const QString Headset = QLatin1String("00001108-0000-1000-8000-00805F9B34FB");
const QString AudioSource = QLatin1String("0000110A-0000-1000-8000-00805F9B34FB");
const QString AudioSink = QLatin1String("0000110B-0000-1000-8000-00805F9B34FB");
const QString AudioVideoRemoteControlTarget = QLatin1String("0000110C-0000-1000-8000-00805F9B34FB");
const QString AdvancedAudioDistribution = QLatin1String("0000110D-0000-1000-8000-00805F9B34FB");
const QString AudioVideoRemoteControl = QLatin1String("0000110E-0000-1000-8000-00805F9B34FB");
const QString HeadsetAudioGateway = QLatin1String("00001112-0000-1000-8000-00805F9B34FB");
const QString Panu = QLatin1String("00001115-0000-1000-8000-00805F9B34FB");
const QString Nap = QLatin1String("00001116-0000-1000-8000-00805F9B34FB");
const QString Handsfree = QLatin1String("0000111E-0000-1000-8000-00805F9B34FB");
const QString HandsfreeAudioGateway = QLatin1String("0000111F-0000-1000-8000-00805F9B34FB");
const QString HumanInterfaceDevice = QLatin1String("00001124-0000-1000-8000-00805F9B34FB");
const QString SimAccess = QLatin1String("0000112D-0000-1000-8000-00805F9B34FB");
const QString PhonebookAccessServer = QLatin1String("0000112F-0000-1000-8000-00805F9B34FB");
const QString MessageAccessServer = QLatin1String("00001132-0000-1000-8000-00805F9B34FB");
const QString PnpInformation = QLatin1String("00001200-0000-1000-8000-00805F9B34FB");

// Bluetooth Low Energy
const QString GenericAcces = QLatin1String("00001800-0000-1000-8000-00805f9b34fb");
const QString GenericAttribute = QLatin1String("00001801-0000-1000-8000-00805f9b34fb");
const QString ImmediateAlert = QLatin1String("00001802-0000-1000-8000-00805f9b34fb");
const QString LinkLoss = QLatin1String("00001803-0000-1000-8000-00805f9b34fb");
const QString TxPower = QLatin1String("00001804-0000-1000-8000-00805f9b34fb");
const QString HeartRate = QLatin1String("0000180d-0000-1000-8000-00805f9b34fb");

}

} // namespace BluezQt

#endif // BLUEZQT_SERVICES_H
