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

#include "mediaendpoint_p.h"

#include "a2dp-codecs.h"
#include "services.h"

namespace BluezQt
{

MediaEndpointPrivate::MediaEndpointPrivate(const MediaEndpoint::Configuration &configuration)
    : m_configuration(configuration)
{
    init(configuration);
}

void MediaEndpointPrivate::init(const MediaEndpoint::Configuration &configuration)
{
    const QString uuid = QStringLiteral("UUID");
    const QString codec = QStringLiteral("Codec");
    const QString capabilities = QStringLiteral("Capabilities");

    QString objectPath = QStringLiteral("/MediaEndpoint");

    switch (configuration.role) {
    case MediaEndpoint::Role::AudioSource:
        m_properties[uuid] = Services::AudioSource;
        objectPath += QStringLiteral("/Source");
        break;
    case MediaEndpoint::Role::AudioSink:
        m_properties[uuid] = Services::AudioSink;
        objectPath += QStringLiteral("/Sink");
        break;
    }

    switch (configuration.codec) {
    case MediaEndpoint::Codec::Sbc:
        m_properties[codec] = QVariant::fromValue(uchar(A2DP_CODEC_SBC));
        m_properties[capabilities] = QByteArray(reinterpret_cast<const char*>(&sbcCapabilities), sizeof(sbcCapabilities));
        objectPath += QStringLiteral("/Sbc");
        break;
    case MediaEndpoint::Codec::Aac:
        m_properties[codec] = QVariant::fromValue(uchar(A2DP_CODEC_MPEG24));
        m_properties[capabilities] = QByteArray(reinterpret_cast<const char*>(&aacCapabilities), sizeof(aacCapabilities));
        objectPath += QStringLiteral("/Aac");
        break;
    }

    m_objectPath.setPath(objectPath);
}

} // namespace BluezQt
