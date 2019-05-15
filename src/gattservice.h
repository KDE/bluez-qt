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

#include "bluezqt_export.h"
#include "types.h"

#include <QDBusObjectPath>

namespace BluezQt
{

/**
 * @class BluezQt::GattService GattService.h <BluezQt/GattService>
 *
 * Bluetooth GattService.
 *
 * This class represents a Bluetooth GattService.
 */
class BLUEZQT_EXPORT GattService : public QObject
{
    Q_OBJECT

public:
    /**
     * Creates a new GattService object.
     *
     * @param parent
     */
    explicit GattService(const QString &uuid, bool isPrimary, GattApplication *parent);

    /**
     * Destroys a GattService object.
     */
    ~GattService();

    /**
     * 128-bit service UUID.
     *
     * @return uuid of gatt service
     */
    QString uuid() const;

    /**
     * Indicates whether or not this GATT service is a
     * primary service. If false, the service is secondary.
     *
     * @return true if gatt service is primary
     */
    bool isPrimary() const;

protected:
    /**
     * D-Bus object path of the GattService.
     *
     * The path where the GattService will be registered.
     *
     * @note You must provide valid object path!
     *
     * @return object path of GattService
     */
    virtual QDBusObjectPath objectPath() const;

private:
    class GattServicePrivate *const d;

    friend class GattApplication;
    friend class GattCharacterisiticPrivate;
    friend class GattCharacteristicAdaptor;
    friend class GattManager;
};

} // namespace BluezQt
