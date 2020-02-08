/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * SPDX-FileCopyrightText: 2019 Manuel Weichselbaumer <mincequi@web.de>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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

    friend class GattApplicationPrivate;
    friend class GattCharacteristicAdaptor;
    friend class GattManager;
};

} // namespace BluezQt
