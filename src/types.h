/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
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

#ifndef BLUEZQT_TYPES_H
#define BLUEZQT_TYPES_H

#include <QSharedPointer>

namespace BluezQt
{

class Manager;
class Adapter;
class Device;
class Input;
class Media;
class MediaPlayer;
class MediaPlayerTrack;
class Agent;
class DevicesModel;
class InitManagerJob;
class Profile;
class PendingCall;
class ObexManager;
class ObexSession;
class ObexTransfer;
class InitObexManagerJob;
class ObexAgent;
class ObexFileTransfer;
class ObexFileTransferEntry;
class ObexObjectPush;

typedef QSharedPointer<BluezQt::Manager> ManagerPtr;
typedef QSharedPointer<BluezQt::Adapter> AdapterPtr;
typedef QSharedPointer<BluezQt::Device> DevicePtr;
typedef QSharedPointer<BluezQt::Input> InputPtr;
typedef QSharedPointer<BluezQt::Media> MediaPtr;
typedef QSharedPointer<BluezQt::MediaPlayer> MediaPlayerPtr;
typedef QSharedPointer<BluezQt::ObexManager> ObexManagerPtr;
typedef QSharedPointer<BluezQt::ObexSession> ObexSessionPtr;
typedef QSharedPointer<BluezQt::ObexTransfer> ObexTransferPtr;

} // namespace BluezQt

#endif // BLUEZQT_TYPES_H
