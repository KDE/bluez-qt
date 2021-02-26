/*
 * BluezQt - Asynchronous Bluez wrapper library
 *
 * SPDX-FileCopyrightText: 2019 Manuel Weichselbaumer <mincequi@web.de>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "gattcharacteristic_p.h"
#include "gattservice.h"

namespace BluezQt
{
GattCharacterisiticPrivate::GattCharacterisiticPrivate(const QString &uuid, const GattService *service)
    : m_uuid(uuid)
    , m_service(service)
{
    static uint8_t charcNumber = 0;
    m_objectPath.setPath(m_service->objectPath().path() + QStringLiteral("/char") + QString::number(charcNumber++));
}

} // namespace BluezQt
