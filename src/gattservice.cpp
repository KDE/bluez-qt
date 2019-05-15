/*
 * BluezQt - Asynchronous Bluez wrapper library
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

#include "gattservice.h"
#include "gattapplication.h"
#include "gattservice_p.h"

namespace BluezQt
{

GattService::GattService(const QString &uuid, bool isPrimary, GattApplication *parent)
    : QObject(parent)
    , d(new GattServicePrivate(uuid, isPrimary, parent->objectPath().path()))
{
}

GattService::~GattService()
{
    delete d;
}

QString GattService::uuid() const
{
    return d->m_uuid;
}

bool GattService::isPrimary() const
{
    return d->m_isPrimary;
}

QDBusObjectPath GattService::objectPath() const
{
    return d->m_objectPath;
}

} // namespace BluezQt
