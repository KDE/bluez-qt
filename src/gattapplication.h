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

#include "bluezqt_dbustypes.h"
#include "bluezqt_export.h"

namespace BluezQt
{

/**
 * @class BluezQt::GattApplication GattApplication.h <BluezQt/GattApplication>
 *
 * Bluetooth GattApplication.
 *
 * This class represents a Bluetooth GattApplication, which is the root node of
 * a GATT object hierarchy. Its child nodes can be GattServices,
 * GattCharacteristics and GattDescriptors that belong to that GattApplication.
 * The object path prefix for GattApplications is freely definable and its
 * children's paths follow the application path hierarchy automatically, while
 * all instances are enumerated automatically as well.
 *
 * Object path: [variable prefix]/appXX/serviceYY/charZZ
 *
 */
class BLUEZQT_EXPORT GattApplication : public QObject
{
    Q_OBJECT

public:
    /**
     * Creates a new GattApplication object with default object path prefix.
     *
     * Object path: /org/kde/bluezqt/appXX/serviceYY/charZZ
     *
     * @param parent
     */
    explicit GattApplication(QObject *parent = nullptr);

    /**
     * Creates a new GattApplication object with custom object path prefix.
     *
     * Object path: [objectPathPrefix]/appXX/serviceYY/charZZ
     *
     * @param objectPathPrefix
     * @param parent
     */
    explicit GattApplication(const QString &objectPathPrefix, QObject *parent = nullptr);

    /**
     * Destroys a GattApplication object.
     */
    ~GattApplication();

private:
    /**
     * D-Bus object path of the GATT application.
     *
     * The path where the GATT application will be registered.
     *
     * @note You must provide valid object path!
     *
     * @return object path of GATT application
     */
    virtual QDBusObjectPath objectPath() const;

    /**
     * Gets all GattServices, GattCharacteristics and GattDescriptors that
     * belong to this GattApplication.
     *
     * The return value of this method is a dict whose keys are object paths.
     * Each value is a dict whose keys are interfaces names. Each value in this
     * inner dict is another dict with property names (as key) and property
     * values (as value).
     */
    DBusManagerStruct getManagedObjects() const;

    class GattApplicationPrivate *const d;

    friend class GattManager;
    friend class GattService;
    friend class ObjectManagerAdaptor;
};

} // namespace BluezQt
