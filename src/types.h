/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * SPDX-FileCopyrightText: 2015 David Rosca <nowrep@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef BLUEZQT_TYPES_H
#define BLUEZQT_TYPES_H

#include <QMap>
#include <QSharedPointer>

/*!
 * \namespace BluezQt
 */
namespace BluezQt
{
class Manager;
class Adapter;
class Battery;
class Device;
class GattServiceRemote;
class GattCharacteristicRemote;
class GattDescriptorRemote;
class Input;
class GattManager;
class LEAdvertisingManager;
class Media;
class MediaPlayer;
class MediaTransport;
class Agent;
class ObexManager;
class ObexSession;
class ObexTransfer;
class ObexFileTransfer;

/*!
 * \typedef BluezQt::ManData
 * Synonym for QMap<uint16_t, QByteArray>.
 */
typedef QMap<uint16_t, QByteArray> ManData;

/*!
 * \typedef BluezQt::ManagerPtr
 * Synonym for QSharedPointer<BluezQt::Manager>.
 */
typedef QSharedPointer<BluezQt::Manager> ManagerPtr;
/*!
 * \typedef BluezQt::AdapterPtr
 * Synonym for QSharedPointer<BluezQt::Adapter>.
 */
typedef QSharedPointer<BluezQt::Adapter> AdapterPtr;
/*!
 * \typedef BluezQt::BatteryPtr
 * Synonym for QSharedPointer<BluezQt::Battery>.
 */
typedef QSharedPointer<BluezQt::Battery> BatteryPtr;
/*!
 * \typedef BluezQt::DevicePtr
 * Synonym for QSharedPointer<BluezQt::Device>.
 */
typedef QSharedPointer<BluezQt::Device> DevicePtr;
/*!
 * \typedef BluezQt::GattServiceRemotePtr
 * Synonym for QSharedPointer<BluezQt::GattServiceRemote>.
 */
typedef QSharedPointer<BluezQt::GattServiceRemote> GattServiceRemotePtr;
/*!
 * \typedef BluezQt::GattCharacteristicRemotePtr
 * Synonym for QSharedPointer<BluezQt::GattCharacteristicRemote>.
 */
typedef QSharedPointer<BluezQt::GattCharacteristicRemote> GattCharacteristicRemotePtr;
/*!
 * \typedef BluezQt::GattDescriptorRemotePtr
 * Synonym for QSharedPointer<BluezQt::GattDescriptorRemote>.
 */
typedef QSharedPointer<BluezQt::GattDescriptorRemote> GattDescriptorRemotePtr;
/*!
 * \typedef BluezQt::InputPtr
 * Synonym for QSharedPointer<BluezQt::Input>.
 */
typedef QSharedPointer<BluezQt::Input> InputPtr;
/*!
 * \typedef BluezQt::GattManagerPtr
 * Synonym for QSharedPointer<BluezQt::GattManager>.
 */
typedef QSharedPointer<BluezQt::GattManager> GattManagerPtr;
/*!
 * \typedef BluezQt::LEAdvertisingManagerPtr
 * Synonym for QSharedPointer<BluezQt::LEAdvertisingManager>.
 */
typedef QSharedPointer<BluezQt::LEAdvertisingManager> LEAdvertisingManagerPtr;
/*!
 * \typedef BluezQt::MediaPtr
 * Synonym for QSharedPointer<BluezQt::Media>.
 */
typedef QSharedPointer<BluezQt::Media> MediaPtr;
/*!
 * \typedef BluezQt::MediaPlayerPtr
 * Synonym for QSharedPointer<BluezQt::MediaPlayer>.
 */
typedef QSharedPointer<BluezQt::MediaPlayer> MediaPlayerPtr;
/*!
 * \typedef BluezQt::MediaTransportPtr
 * Synonym for QSharedPointer<BluezQt::MediaTransport>.
 */
typedef QSharedPointer<BluezQt::MediaTransport> MediaTransportPtr;
/*!
 * \typedef BluezQt::ObexManagerPtr
 * Synonym for QSharedPointer<BluezQt::ObexManager>.
 */
typedef QSharedPointer<BluezQt::ObexManager> ObexManagerPtr;
/*!
 * \typedef BluezQt::ObexSessionPtr
 * Synonym for QSharedPointer<BluezQt::ObexSession>.
 */
typedef QSharedPointer<BluezQt::ObexSession> ObexSessionPtr;
/*!
 * \typedef BluezQt::ObexTransferPtr
 * Synonym for QSharedPointer<BluezQt::ObexTransfer>.
 */
typedef QSharedPointer<BluezQt::ObexTransfer> ObexTransferPtr;

} // namespace BluezQt

#endif // BLUEZQT_TYPES_H
