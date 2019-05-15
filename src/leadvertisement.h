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

#ifndef BLUEZQT_LEADVERTISEMENT_H
#define BLUEZQT_LEADVERTISEMENT_H

#include <QObject>

#include "bluezqt_export.h"

class QDBusObjectPath;

namespace BluezQt
{

/**
 * @class BluezQt::LEAdvertisement leadvertisement.h <BluezQt/LEAdvertisement>
 *
 * Bluetooth LE advertisement.
 *
 * This class represents a Bluetooth LE advertisement.
 */
class BLUEZQT_EXPORT LEAdvertisement : public QObject
{
    Q_OBJECT

public:
    /**
     * Creates a new LEAdvertisement object.
     *
     * @param parent
     */
    explicit LEAdvertisement(const QStringList &serviceUuids, QObject *parent = nullptr);

    /**
     * Destroys a LEAdvertisement object.
     */
    ~LEAdvertisement();

    /**
     * List of UUIDs to include in the "Service UUID" field of the Advertising Data.
     *
     * @return UUIDs of the advertisement
     */
    virtual QStringList serviceUuids() const;

    /**
     * Indicates that the LEAdvertisement was unregistered.
     *
     * This method gets called when the service daemon removes the Advertisement.
     * A client can use it to do cleanup tasks. There is no need to call
     * UnregisterAdvertisement because when this method gets called it has
     * already been unregistered.
     */
    virtual void release();

protected:
    /**
     * D-Bus object path of the advertisement.
     *
     * The path where the advertisement will be registered.
     *
     * @note You must provide valid object path!
     *
     * @return object path of advertisement
     */
    virtual QDBusObjectPath objectPath() const;

private:
    class LEAdvertisementPrivate *const d;

    friend class LEAdvertisingManager;
};

} // namespace BluezQt

#endif // BLUEZQT_LEADVERTISEMENT_H
