/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * SPDX-FileCopyrightText: 2019 Manuel Weichselbaumer <mincequi@web.de>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef BLUEZQT_GATTCHARACTERISTIC_H
#define BLUEZQT_GATTCHARACTERISTIC_H

#include "bluezqt_export.h"

#include <QDBusObjectPath>

#include <memory>

namespace BluezQt
{
class GattService;

/*!
 * \inmodule BluezQt
 * \class BluezQt::GattCharacteristic
 * \inheaderfile BluezQt/GattCharacteristic
 */
class BLUEZQT_EXPORT GattCharacteristic : public QObject
{
    Q_OBJECT

public:
    /*!
     * Creates a new GattCharacteristic object with the given \a uuid and a parent \a service.
     *
     * This constructor creates a characteristic with the Read and Write flags set.
     */
    explicit GattCharacteristic(const QString &uuid, GattService *service);

    /*!
     * Creates a new GattCharacteristic object with the given \a uuid and a parent \a service.
     *
     * The \a flags can be used to indicate the characteristic usage.
     *
     * \since 6.0
     */
    GattCharacteristic(const QString &uuid, const QStringList &flags, GattService *service);

    ~GattCharacteristic() override;

    /*!
     * Reads the value of the characteristic.
     */
    QByteArray readValue();

    /*!
     * Writes the \a value of the characteristic.
     */
    void writeValue(const QByteArray &value);

    /*!
     * Provide a read \a callback to operate in \e pull mode.
     */
    using ReadCallback = std::function<QByteArray()>;
    void setReadCallback(ReadCallback callback);

    /*!
     * Returns the 128-bit GATT characteristic UUID.
     */
    QString uuid() const;

    /*!
     * The GATT service the characteristic belongs to.
     */
    const GattService *service() const;

    /*!
     * Returns flags associated with this characteristic.
     * \since 6.0
     */
    QStringList flags() const;

    /*!
     * Enables notifications for this characteristic, if supported. Does nothing otherwise.
     * \since 6.0
     */
    void startNotify();

    /*!
     * Disables notifications for this characteristic.
     * \since 6.0
     */
    void stopNotify();

    /*!
     * Indicates if this characteristic currently has notifications enabled.
     * \since 6.0
     */
    bool isNotifying() const;

Q_SIGNALS:
    /*!
     * Indicates that a \a value was written.
     */
    void valueWritten(const QByteArray &value);

protected:
    /*!
     * Returns the D-Bus object path of the GattCharacteristic.
     *
     * The path where the GattCharacteristic will be registered.
     *
     * \note You must provide valid object path!
     */
    virtual QDBusObjectPath objectPath() const;

private:
    std::unique_ptr<class GattCharacterisiticPrivate> const d;

    friend class GattApplicationPrivate;
    friend class GattCharacteristicAdaptor;
    friend class GattDescriptor;
    friend class GattDescriptorPrivate;
    friend class GattManager;
};

} // namespace BluezQt

#endif
