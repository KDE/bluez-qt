/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * SPDX-FileCopyrightText: 2019 Manuel Weichselbaumer <mincequi@web.de>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include <QDBusObjectPath>

namespace BluezQt
{
class GattServicePrivate
{
public:
    GattServicePrivate(const QString &uuid, bool isPrimary, const QString &appPath);

    QString m_uuid;
    bool m_isPrimary;
    QDBusObjectPath m_objectPath;
};

} // namespace BluezQt
