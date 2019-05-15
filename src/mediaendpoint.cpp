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

#include "mediaendpoint.h"
#include "mediaendpoint_p.h"
#include "a2dp-codecs.h"

namespace BluezQt
{

MediaEndpoint::MediaEndpoint(const Configuration &configuration, QObject *parent)
    : QObject(parent),
      d(new MediaEndpointPrivate(configuration))
{
}

MediaEndpoint::~MediaEndpoint()
{
    delete d;
}

QDBusObjectPath MediaEndpoint::objectPath() const
{
    return d->m_objectPath;
}

const QVariantMap &MediaEndpoint::properties() const
{
    return d->m_properties;
}

void MediaEndpoint::setConfiguration(const QString &transportObjectPath, const QVariantMap &properties)
{
    emit configurationSet(transportObjectPath, properties);
}

void MediaEndpoint::selectConfiguration(const QByteArray &capabilities, const Request<QByteArray> &request)
{
    switch (d->m_configuration.codec) {
    case MediaEndpoint::Codec::Sbc:
    {
        if (capabilities.size() != sizeof(a2dp_sbc_t)) {
            emit configurationSelected(capabilities, QByteArray());
            request.reject();
            return;
        }

        a2dp_sbc_t caps = *reinterpret_cast<const a2dp_sbc_t*>(capabilities.constData());
        if (caps.frequency & SBC_SAMPLING_FREQ_44100) {
            caps.frequency = SBC_SAMPLING_FREQ_44100;
        } else if (caps.frequency & SBC_SAMPLING_FREQ_48000) {
            caps.frequency = SBC_SAMPLING_FREQ_48000;
        } else {
            break;
        }

        if (caps.channel_mode & SBC_CHANNEL_MODE_STEREO) {
            caps.channel_mode = SBC_CHANNEL_MODE_STEREO;
        } else if (caps.channel_mode & SBC_CHANNEL_MODE_JOINT_STEREO) {
            caps.channel_mode = SBC_CHANNEL_MODE_JOINT_STEREO;
        } else {
            break;
        }

        if (caps.block_length & SBC_BLOCK_LENGTH_16) {
            caps.block_length = SBC_BLOCK_LENGTH_16;
        } else if (caps.block_length & SBC_BLOCK_LENGTH_12) {
            caps.block_length = SBC_BLOCK_LENGTH_12;
        } else if (caps.block_length & SBC_BLOCK_LENGTH_8) {
            caps.block_length = SBC_BLOCK_LENGTH_8;
        } else if (caps.block_length & SBC_BLOCK_LENGTH_4) {
            caps.block_length = SBC_BLOCK_LENGTH_4;
        } else {
            break;
        }

        if (caps.subbands & SBC_SUBBANDS_8) {
            caps.subbands = SBC_SUBBANDS_8;
        } else if (caps.subbands & SBC_SUBBANDS_4) {
            caps.subbands = SBC_SUBBANDS_4;
        } else {
            break;
        }

        if (caps.allocation_method & SBC_ALLOCATION_LOUDNESS) {
            caps.allocation_method = SBC_ALLOCATION_LOUDNESS;
        } else if (caps.allocation_method & SBC_ALLOCATION_SNR) {
            caps.allocation_method = SBC_ALLOCATION_SNR;
        } else {
            break;
        }

        caps.min_bitpool = 2;
        caps.max_bitpool = 53;

        const QByteArray configuration(reinterpret_cast<const char*>(&caps), sizeof(caps));
        emit configurationSelected(capabilities, configuration);
        request.accept(configuration);
        return;

        break;
    }
    case MediaEndpoint::Codec::Aac:
        if (capabilities.size() != sizeof(a2dp_aac_t)) {
            emit configurationSelected(capabilities, QByteArray());
            request.reject();
            return;
        }

        // TODO: implement AAC. However selectConfiguration seems not to be used by iOS nor Android.
        emit configurationSelected(capabilities, QByteArray());
        request.reject();
        return;

        break;
    }

    emit configurationSelected(capabilities, QByteArray());
    request.reject();
}

void MediaEndpoint::clearConfiguration(const QString &transportObjectPath)
{
    emit configurationCleared(transportObjectPath);
}

void MediaEndpoint::release()
{
}

} // namespace BluezQt
