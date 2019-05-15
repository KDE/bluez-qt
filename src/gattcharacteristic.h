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

#include <QDBusObjectPath>

namespace BluezQt
{
class GattService;

class BLUEZQT_EXPORT GattCharacteristic : public QObject
{
    Q_OBJECT

public:
    /**
     * Creates a new GattCharacteristic object.
     *
     * @param parent
     */
    explicit GattCharacteristic(const QString &uuid, GattService *service);

    /**
     * Destroys a GattCharacteristic object.
     */
    ~GattCharacteristic();

    /**
     * Reads the value of the characteristic.
     */
    QByteArray readValue();

    /**
     * Writes the value of the characteristic.
     */
    void writeValue(const QByteArray &value);

    /**
     * Provide a read callback to operate in *pull* mode.
     */
    using ReadCallback = std::function<QByteArray()>;
    void setReadCallback(ReadCallback callback);

    /**
     * 128-bit GATT characteristic UUID.
     *
     * @return uuid of characteristic
     */
    QString uuid() const;

    /**
     * The GATT service the characteristic belongs to.
     *
     * @return service this characteristic belongs to
     */
    const GattService *service() const;

Q_SIGNALS:
    /**
     * Indicates that a value was written.
     */
    void valueWritten(const QByteArray &value);

protected:
    /**
     * D-Bus object path of the GattCharacteristic.
     *
     * The path where the GattCharacteristic will be registered.
     *
     * @note You must provide valid object path!
     *
     * @return object path of GattCharacteristic
     */
    virtual QDBusObjectPath objectPath() const;

private:
    class GattCharacterisiticPrivate *const d;

    friend class GattApplication;
    friend class GattCharacteristicAdaptor;
    friend class GattManager;
};

} // namespace BluezQt
