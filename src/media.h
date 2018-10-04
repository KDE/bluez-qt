/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * Copyright (C) 2018 Manuel Weichselbaumer <mincequi@web.de>
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

#ifndef BLUEZQT_MEDIA_H
#define BLUEZQT_MEDIA_H

#include <QObject>

#include "bluezqt_export.h"

namespace BluezQt
{

class MediaEndpoint;
class PendingCall;

/**
 * @class BluezQt::Media Media.h <BluezQt/Media>
 *
 * Bluetooth Media.
 *
 * This allows media endpoints to be established in accordance with the
 * capabilities of a specific media service profile.
 *
 * For example, an A2DP media endpoint could be created allowing data from a
 * remote device to be streamed to/from the sender.
 *
 * Each media endpoint is associated with a service object instance that
 * implements the required behaviours of the endpoint. The service object
 * must be created at a given path before it is registered.
 *
 * @see MediaEndpoint
 */
class BLUEZQT_EXPORT Media : public QObject
{
    Q_OBJECT

public:
    /**
     * Destroys a Media object.
     */
    ~Media();

    /**
     * Registers endpoint.
     *
     * Register a local end point to sender, the sender can register as many end points as it likes.
     *
     * Note: If the sender disconnects the end points are automatically unregistered.
     *
     * Possible errors: PendingCall::InvalidArguments, PendingCall::NotSupported
     *
     * @param endpoint endpoint to be registered
     * @return void pending call
     */
    PendingCall *registerEndpoint(MediaEndpoint *endpoint);

    /**
     * Unregisters endpoint.
     *
     * @param endpoint endpoint to be unregistered
     * @return void pending call
     */
    PendingCall *unregisterEndpoint(MediaEndpoint *endpoint);

private:
    explicit Media(const QString &path, QObject *parent = nullptr);

    class MediaPrivate *const d;

    friend class ManagerPrivate;
};

} // namespace BluezQt

#endif // BLUEZQT_MEDIA_H
