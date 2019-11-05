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

#include "a2dp-codecs.h"

/* Currently our capabilities are limited to the most common use cases.
   iOS has a fixed stream configuration anyway (for SBC and AAC). */
const a2dp_sbc_t sbcCapabilities = {
    .frequency =
    /*SBC_SAMPLING_FREQ_16000 |
    SBC_SAMPLING_FREQ_32000 |*/
    SBC_SAMPLING_FREQ_44100 |
    SBC_SAMPLING_FREQ_48000,
    .channel_mode =
    /*SBC_CHANNEL_MODE_MONO |
    SBC_CHANNEL_MODE_DUAL_CHANNEL |*/
    SBC_CHANNEL_MODE_STEREO |
    SBC_CHANNEL_MODE_JOINT_STEREO,
    .block_length =
    SBC_BLOCK_LENGTH_4 |
    SBC_BLOCK_LENGTH_8 |
    SBC_BLOCK_LENGTH_12 |
    SBC_BLOCK_LENGTH_16,
    .subbands =
    SBC_SUBBANDS_4 |
    SBC_SUBBANDS_8,
    .allocation_method =
    SBC_ALLOCATION_SNR |
    SBC_ALLOCATION_LOUDNESS,
    .min_bitpool = MIN_BITPOOL,
    .max_bitpool = MAX_BITPOOL,
};

const a2dp_aac_t aacCapabilities = {
    .object_type =
    AAC_OBJECT_TYPE_MPEG2_AAC_LC |
    AAC_OBJECT_TYPE_MPEG4_AAC_LC,
    /*AAC_OBJECT_TYPE_MPEG4_AAC_LTP |
    AAC_OBJECT_TYPE_MPEG4_AAC_SCA, */
    AAC_INIT_FREQUENCY(
    /*AAC_SAMPLING_FREQ_8000 |
    AAC_SAMPLING_FREQ_11025 |
    AAC_SAMPLING_FREQ_12000 |
    AAC_SAMPLING_FREQ_16000 |
    AAC_SAMPLING_FREQ_22050 |
    AAC_SAMPLING_FREQ_24000 |
    AAC_SAMPLING_FREQ_32000 |*/
    AAC_SAMPLING_FREQ_44100 |
    AAC_SAMPLING_FREQ_48000)
    /*AAC_SAMPLING_FREQ_64000 |
    AAC_SAMPLING_FREQ_88200 |
    AAC_SAMPLING_FREQ_96000)*/
    .channels =
    /*AAC_CHANNELS_1 |*/
    AAC_CHANNELS_2,
    .vbr = 1,
    AAC_INIT_BITRATE(0xFFFF)
};
