/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * Copyright (C) 2019 Manuel Weichselbaumer <mincequi@web.de>
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

#pragma once

#include <QObject>

#include "bluezqt_dbustypes.h"
#include "bluezqt_export.h"

namespace BluezQt
{

class GattApplication;
class PendingCall;

/**
 * @class BluezQt::GattManager GattManager.h <BluezQt/GattManager>
 *
 * Bluetooth GattManager.
 *
 * GATT Manager allows external applications to register GATT services and
 * profiles.
 *
 * Registering a profile allows applications to subscribe to *remote* services.
 * These must implement the GattProfile1 interface defined above.
 *
 * Registering a service allows applications to publish a *local* GATT service,
 * which then becomes available to remote devices. A GATT service is represented by
 * a D-Bus object hierarchy where the root node corresponds to a service and the
 * child nodes represent characteristics and descriptors that belong to that
 * service. Each node must implement one of GattService1, GattCharacteristic1,
 * or GattDescriptor1 interfaces described above, based on the attribute it
 * represents. Each node must also implement the standard D-Bus Properties
 * interface to expose their properties. These objects collectively represent a
 * GATT service definition.
 *
 * @see GattApplication
 */
class BLUEZQT_EXPORT GattManager : public QObject
{
    Q_OBJECT

public:
    /**
     * Destroys a GattManager object.
     */
    ~GattManager();

    /**
     * Registers a local GATT services hierarchy as described
     * above (GATT Server) and/or GATT profiles (GATT Client).
     *
     * The application object path together with the D-Bus
     * system bus connection ID define the identification of
     * the application registering a GATT based
     * service or profile.
     *
     * Possible errors: org.bluez.Error.InvalidArguments
     *             org.bluez.Error.AlreadyExists
     *
     * @param application application to be registered
     * @return void pending call
     */
    PendingCall *registerApplication(GattApplication *application);

    /**
     * This unregisters the services that has been
     * previously registered. The object path parameter
     * must match the same value that has been used
     * on registration.
     *
     * Possible errors: org.bluez.Error.InvalidArguments
     *             org.bluez.Error.DoesNotExist
     *
     * @param application application to be unregistered
     * @return void pending call
     */
    PendingCall *unregisterApplication(GattApplication *application);

private:
    explicit GattManager(const QString &path, QObject *parent = nullptr);

    class GattManagerPrivate *const d;

    friend class AdapterPrivate;
};

} // namespace BluezQt

