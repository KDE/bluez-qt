/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * SPDX-FileCopyrightText: 2019 Manuel Weichselbaumer <mincequi@web.de>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "gattcharacteristic.h"

namespace BluezQt
{
class GattServicePrivate;

class GattCharacterisiticPrivate
{
public:
    GattCharacterisiticPrivate(const QString &uuid, const GattService *service);

    QString m_uuid;
    const GattService *m_service;
    QDBusObjectPath m_objectPath;
    QByteArray m_value;
    GattCharacteristic::ReadCallback m_readCallback = nullptr;
};

} // namespace BluezQt
