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

#ifndef BLUEZQT_MANAGER_H
#define BLUEZQT_MANAGER_H

#include <QObject>

#include "types.h"
#include <bluezqt_export.h>

namespace BluezQt
{

class Device;
class Adapter;
class Agent;
class Profile;
class PendingCall;
class InitManagerJob;

/**
 * Bluetooth manager.
 *
 * The entry point to communicate with system Bluez daemon.
 *
 * The typical usecase is to work with usableAdapter() (any powered adapter),
 * but it is also possible to use specific adapter.
 *
 * You must call init() before other functions can be used.
 *
 * The only functions that can be used before initialization are rfkill-related functions:
 * isBluetoothBlocked(), setBluetoothBlocked() and bluetoothBlockedChanged() signal.
 *
 * Example use in C++ code:
 * @code
 * BluezQt::Manager *manager = new BluezQt::Manager();
 * BluezQt::InitManagerJob *job = manager->init();
 * job->start();
 * connect(job, &BluezQt::InitManagerJob::result, ...);
 * @endcode
 *
 * In QML, manager is automatically starting initialization when instantiated.
 *
 * There are 2 new signals: initFinished() and initError(string errorText).

 * Example use in QML code:
 * @code
 * import org.kde.bluezqt 1.0 as BluezQt
 *
 * BluezQt.Manager {
 *     id: btManager;
 *
 *     onInitFinished: {
 *         console.log("Initialized");
 *     }
 *
 *     onInitError: {
 *         console.log("Error: ", errorText);
 *     }
 * }
 * @endcode
 *
 * @note All communication with Bluez daemon happens asynchronously. Almost all methods
 *       returns PendingCall to help track the call progress and to check for any errors.
 *
 * @note If manager is not operational, all methods that returns a PendingCall
 *       will fail with PendingCall::InternalError.
 *
 * @see InitManagerJob
 */
class BLUEZQT_EXPORT Manager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(BluezQt::AdapterPtr usableAdapter READ usableAdapter NOTIFY usableAdapterChanged)
    Q_PROPERTY(QList<BluezQt::AdapterPtr> adapters READ adapters)
    Q_PROPERTY(QList<BluezQt::DevicePtr> devices READ devices)
    Q_PROPERTY(bool initialized READ isInitialized)
    Q_PROPERTY(bool operational READ isOperational NOTIFY operationalChanged)
    Q_PROPERTY(bool bluetoothOperational READ isBluetoothOperational NOTIFY bluetoothOperationalChanged)
    Q_PROPERTY(bool bluetoothBlocked READ isBluetoothBlocked WRITE setBluetoothBlocked NOTIFY bluetoothBlockedChanged)

public:
    /**
     * Creates a new Manager object.
     *
     * @param parent
     */
    explicit Manager(QObject *parent = 0);

    /**
     * Destroys a Manager object.
     */
    ~Manager();

    /**
     * Creates a new init job.
     *
     * @return init manager job
     */
    InitManagerJob *init();

    /**
     * Returns whether the manager is initialized.
     *
     * @return true if manager is initialized
     */
    bool isInitialized() const;

    /**
     * Returns whether the manager is operational.
     *
     * The manager is operational when initialization was successful and
     * Bluez system daemon is running.
     *
     * @return true if manager is operational
     */
    bool isOperational() const;

    /**
     * Returns whether Bluetooth is operational.
     *
     * Bluetooth is operational when manager is operational and there is
     * a valid usable adapter.
     *
     * @return true if Bluetooth is operational
     */
    bool isBluetoothOperational() const;

    /**
     * Returns whether Bluetooth is blocked.
     *
     * Bluetooth is blocked if rfkill state for Bluetooth is either
     * SOFT_BLOCKED or HARD_BLOCKED.
     *
     * @note This requires read access to /dev/rfkill.
     *
     * @return true if Bluetooth is blocked
     */
    bool isBluetoothBlocked() const;

    /**
     * Sets a Bluetooth blocked state.
     *
     * This may fail either due to insufficent permissions or
     * because rfkill state is HARD_BLOCKED. In that case,
     * this function returns false.
     *
     * @note This requires write access to /dev/rfkill.
     *
     * @return true if Bluetooth blocked state was changed
     */
    bool setBluetoothBlocked(bool blocked);

    /**
     * Returns a usable adapter.
     *
     * Usable adapter is any adapter that is currently powered on.
     *
     * @return usable adapter
     */
    AdapterPtr usableAdapter() const;

    /**
     * Returns a list of all adapters.
     *
     * @return list of adapters
     */
    QList<AdapterPtr> adapters() const;

    /**
     * Returns a list of all devices.
     *
     * @return list of devices
     */
    QList<DevicePtr> devices() const;

    /**
     * Attempts to start org.bluez service by D-Bus activation.
     *
     * Possible return values are 1 if the service was started,
     * 2 if the service is already running or error if the service
     * could not be started.
     *
     * @return quint32 pending call
     */
    static BluezQt::PendingCall *startService();

    /**
     * Returns an adapter for specified address.
     *
     * @param address address of adapter (eg. "1C:E5:C3:BC:94:7E")
     * @return null if there is no adapter with specified address
     */
    BluezQt::AdapterPtr adapterForAddress(const QString &address) const;

    /**
     * Returns an adapter for specified UBI.
     *
     * @param ubi UBI of adapter (eg. "/org/bluez/hci0")
     * @return null if there is no adapter with specified UBI
     */
    BluezQt::AdapterPtr adapterForUbi(const QString &ubi) const;

    /**
     * Returns a device for specified address.
     *
     * @param address address of device (eg. "40:79:6A:0C:39:75")
     * @return null if there is no device with specified address
     *
     * @note There may be more devices with the same address (same device
     *       in multiple adapters). In this case, the first found device will
     *       be returned while preferring powered adapters in search.
     */
    BluezQt::DevicePtr deviceForAddress(const QString &address) const;

    /**
     * Returns a device for specified UBI.
     *
     * @param ubi UBI of device (eg. "/org/bluez/hci0/dev_40_79_6A_0C_39_75")
     * @return null if there is no device with specified UBI
     */
    BluezQt::DevicePtr deviceForUbi(const QString &ubi) const;

    /**
     * Registers agent.
     *
     * This agent will be used for for all actions triggered by the application.
     * Eg. show a PIN code in pairing process.
     *
     * Possible errors: PendingCall::InvalidArguments, PendingCall::AlreadyExists
     *
     * @param agent agent to be registered
     * @return void pending call
     */
    PendingCall *registerAgent(Agent *agent);

    /**
     * Unregisters agent.
     *
     * Possible errors: PendingCall::DoesNotExist
     *
     * @param agent agent to be unregistered
     * @return void pending call
     */
    PendingCall *unregisterAgent(Agent *agent);

    /**
     * Requests default agent.
     *
     * This requests to make the application agent the default agent.
     *
     * Possible errors: PendingCall::DoesNotExist
     *
     * @param agent registered agent
     * @return void pending call
     */
    PendingCall *requestDefaultAgent(Agent *agent);

    /**
     * Registers profile.
     *
     * Possible errors: PendingCall::InvalidArguments, PendingCall::AlreadyExists
     *
     * @param profile profile to be registered
     * @return void pending call
     */
    PendingCall *registerProfile(Profile *profile);

    /**
     * Unregisters profile.
     *
     * Possible errors: PendingCall::DoesNotExists
     *
     * @param profile profile to be unregistered
     * @return void pending call
     */
    PendingCall *unregisterProfile(Profile *profile);

Q_SIGNALS:
    /**
     * Indicates that operational state have changed.
     */
    void operationalChanged(bool operational);

    /**
     * Indicates that Bluetooth operational state have changed.
     */
    void bluetoothOperationalChanged(bool operational);

    /**
     * Indicates that Bluetooth blocked state have changed.
     */
    void bluetoothBlockedChanged(bool blocked);

    /**
     * Indicates that adapter was added.
     */
    void adapterAdded(BluezQt::AdapterPtr adapter);

    /**
     * Indicates that adapter was removed.
     */
    void adapterRemoved(BluezQt::AdapterPtr adapter);

    /**
     * Indicates that at least one of the adapter's properties have changed.
     */
    void adapterChanged(BluezQt::AdapterPtr adapter);

    /**
     * Indicates that a new device was added (eg. found by discovery).
     */
    void deviceAdded(BluezQt::DevicePtr device);

    /**
     * Indicates that a device was removed.
     */
    void deviceRemoved(BluezQt::DevicePtr device);

    /**
     * Indicates that at least one of the device's properties have changed.
     */
    void deviceChanged(BluezQt::DevicePtr device);

    /**
     * Indicates that usable adapter have changed.
     */
    void usableAdapterChanged(BluezQt::AdapterPtr adapter);

    /**
     * Indicates that all adapters were removed.
     */
    void allAdaptersRemoved();

private:
    class ManagerPrivate *const d;

    friend class ManagerPrivate;
    friend class InitManagerJobPrivate;
};

} // namespace BluezQt

#endif // BLUEZQT_MANAGER_H
