/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * SPDX-FileCopyrightText: 2015 David Rosca <nowrep@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef BLUEZQT_TYPES_H
#define BLUEZQT_TYPES_H

#include <QSharedPointer>

namespace BluezQt
{
class Manager;
class Adapter;
class Battery;
class Device;
class Input;
class GattManager;
class LEAdvertisingManager;
class Media;
class MediaPlayer;
class MediaPlayerTrack;
class MediaTransport;
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
class GattApplication;

typedef QSharedPointer<BluezQt::Manager> ManagerPtr;
typedef QSharedPointer<BluezQt::Adapter> AdapterPtr;
typedef QSharedPointer<BluezQt::Battery> BatteryPtr;
typedef QSharedPointer<BluezQt::Device> DevicePtr;
typedef QSharedPointer<BluezQt::Input> InputPtr;
typedef QSharedPointer<BluezQt::GattManager> GattManagerPtr;
typedef QSharedPointer<BluezQt::LEAdvertisingManager> LEAdvertisingManagerPtr;
typedef QSharedPointer<BluezQt::Media> MediaPtr;
typedef QSharedPointer<BluezQt::MediaPlayer> MediaPlayerPtr;
typedef QSharedPointer<BluezQt::MediaTransport> MediaTransportPtr;
typedef QSharedPointer<BluezQt::ObexManager> ObexManagerPtr;
typedef QSharedPointer<BluezQt::ObexSession> ObexSessionPtr;
typedef QSharedPointer<BluezQt::ObexTransfer> ObexTransferPtr;

} // namespace BluezQt

#endif // BLUEZQT_TYPES_H
