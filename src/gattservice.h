/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * SPDX-FileCopyrightText: 2019 Manuel Weichselbaumer <mincequi@web.de>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef BLUEZQT_GATTSERVICE_H
#define BLUEZQT_GATTSERVICE_H

#include "bluezqt_export.h"
#include "types.h"

#include <QDBusObjectPath>

#include <memory>

namespace BluezQt
{
class GattApplication;
/*!
 * \inmodule BluezQt
 * \class BluezQt::GattService
 * \inheaderfile BluezQt/GattService
 * \brief Bluetooth GattService.
 *
 * This class represents a Bluetooth GattService.
 */
class BLUEZQT_EXPORT GattService : public QObject
{
    Q_OBJECT

public:
    /*!
     * Creates a new GattService object as a child of a \a parent application.
     */
    explicit GattService(const QString &uuid, bool isPrimary, GattApplication *parent);

    ~GattService() override;

    /*!
     * Returns the 128-bit service UUID.
     */
    QString uuid() const;

    /*!
     * Indicates whether or not this GATT service is a
     * primary service.
     *
     * Returns \c true if this GATT service is primary, \c false if secondary.
     */
    bool isPrimary() const;

protected:
    /*!
     * Returns the D-Bus object path registered for the GattService.
     *
     * \note You must provide valid object path!
     */
    virtual QDBusObjectPath objectPath() const;

private:
    std::unique_ptr<class GattServicePrivate> const d;

    friend class GattApplicationPrivate;
    friend class GattCharacterisiticPrivate;
    friend class GattCharacteristicAdaptor;
    friend class GattManager;
};

} // namespace BluezQt

#endif
