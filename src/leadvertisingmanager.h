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

#ifndef BLUEZQT_LEADVERTISINGMANAGER_H
#define BLUEZQT_LEADVERTISINGMANAGER_H

#include <QObject>

#include "bluezqt_export.h"

namespace BluezQt
{

class LEAdvertisement;
class PendingCall;

/**
 * @class BluezQt::LEAdvertisingManager leadvertisingmanager.h <BluezQt/LEAdvertisingManager>
 *
 * Bluetooth LE advertising manager.
 *
 * The Advertising Manager allows external applications to register Advertisement
 * Data which should be broadcast to devices. Advertisement Data elements must
 * follow the API for LE Advertisement Data.
 *
 * @see LEAdvertisement
 */
class BLUEZQT_EXPORT LEAdvertisingManager : public QObject
{
    Q_OBJECT

public:
    /**
     * Destroys an LEAdvertisingManager object.
     */
    ~LEAdvertisingManager();

    /**
     * Registers advertisement.
     *
     * Registers an advertisement object to be sent over the LE Advertising
     * channel. The service must be exported under interface LEAdvertisement1.
     *
     * InvalidArguments error indicates invalid or conflicting properties.
     * InvalidLength error indicates that provided data results in too long data packet.
     * The properties of this object are parsed on register, any changes are ignored.
     * If the same object is registered twice it will result in an AlreadyExists error.
     * NotPermitted error indicates that the maximum number of advertisements is reached.
     *
     * Possible errors: PendingCall::InvalidArguments, PendingCall::AlreadyExists,
     *                  PendingCall::InvalidLength, PendingCall::NotPermitted
     *
     * @param advertisement advertisement to be registered
     * @return void pending call
     */
    PendingCall *registerAdvertisement(LEAdvertisement *advertisement);

    /**
     * Unregisters advertisement.
     *
     * This unregisters an advertisement that has been previously registered.
     * The object path must match the same value that has been used on registration.
     *
     * Possible errors: PendingCall::InvalidArguments, PendingCall::DoesNotExist
     *
     * @param advertisement advertisement to be unregistered
     * @return void pending call
     */
    PendingCall *unregisterAdvertisement(LEAdvertisement *advertisement);

private:
    explicit LEAdvertisingManager(const QString &path, QObject *parent = nullptr);

    class LEAdvertisingManagerPrivate *const d;

    friend class AdapterPrivate;
};

} // namespace BluezQt

#endif // BLUEZQT_LEADVERTISINGMANAGER_H
