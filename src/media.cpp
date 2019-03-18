/*
 * BluezQt - Asynchronous Bluez wrapper library
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

#include "media.h"
#include "media_p.h"
#include "mediaendpoint.h"
#include "mediaendpointadaptor.h"
#include "pendingcall.h"
#include "utils.h"
#include "debug.h"

namespace BluezQt
{

Media::Media(const QString &path, QObject *parent)
    : QObject(parent)
    , d(new MediaPrivate())
{
    d->m_path = path;
    d->m_bluezMedia = new BluezMedia(Strings::orgBluez(), path, DBusConnection::orgBluez(), this);
}

Media::~Media()
{
    delete d;
}

PendingCall *Media::registerEndpoint(MediaEndpoint *endpoint)
{
    Q_ASSERT(endpoint);

    if (!d->m_bluezMedia) {
        return new PendingCall(PendingCall::InternalError, QStringLiteral("Media not operational!"));
    }

    new MediaEndpointAdaptor(endpoint);

    if (!DBusConnection::orgBluez().registerObject(endpoint->objectPath().path(), endpoint)) {
        qCDebug(BLUEZQT) << "Cannot register object" << endpoint->objectPath().path();
    }

    return new PendingCall(d->m_bluezMedia->RegisterEndpoint(endpoint->objectPath(), endpoint->properties()),
                           PendingCall::ReturnVoid, this);
}

PendingCall *Media::unregisterEndpoint(MediaEndpoint *endpoint)
{
    Q_ASSERT(endpoint);

    if (!d->m_bluezMedia) {
        return new PendingCall(PendingCall::InternalError, QStringLiteral("Media not operational!"));
    }

    DBusConnection::orgBluez().unregisterObject(endpoint->objectPath().path());

    return new PendingCall(d->m_bluezMedia->UnregisterEndpoint(endpoint->objectPath()),
                           PendingCall::ReturnVoid, this);
}

} // namespace BluezQt
