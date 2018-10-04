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

#include "mediaendpointadaptor.h"
#include "mediaendpoint.h"
#include "request.h"

#include <QDBusMessage>
#include <QDBusObjectPath>

namespace BluezQt
{

MediaEndpointAdaptor::MediaEndpointAdaptor(MediaEndpoint *parent)
    : QDBusAbstractAdaptor(parent)
    , m_endpoint(parent)
{
}

void MediaEndpointAdaptor::SetConfiguration(const QDBusObjectPath &transport, const QVariantMap &properties)
{
    m_endpoint->setConfiguration(transport.path(), properties);
}

QByteArray MediaEndpointAdaptor::SelectConfiguration(const QByteArray &capabilities, const QDBusMessage &msg)
{
    msg.setDelayedReply(true);
    Request<QByteArray> req(OrgBluezMediaEndpoint, msg);

    m_endpoint->selectConfiguration(capabilities, req);
    return QByteArray();
}

void MediaEndpointAdaptor::ClearConfiguration(const QDBusObjectPath &transport)
{
    m_endpoint->clearConfiguration(transport.path());
}

void MediaEndpointAdaptor::Release()
{
    m_endpoint->release();
}

} // namespace BluezQt
