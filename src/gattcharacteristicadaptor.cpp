/*
 * BluezQt - Asynchronous Bluez wrapper library
 *
 * Copyright (C) 2019 Manuel Weichselbaumer
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

#include "gattcharacteristicadaptor.h"
#include "gattcharacteristic.h"
#include "gattservice.h"

#include <QDebug>

namespace BluezQt
{

GattCharacteristicAdaptor::GattCharacteristicAdaptor(GattCharacteristic *parent)
    : QDBusAbstractAdaptor(parent)
    , m_gattCharacteristic(parent)
{
}

QString GattCharacteristicAdaptor::uuid() const
{
    return m_gattCharacteristic->uuid();
}

QDBusObjectPath GattCharacteristicAdaptor::service() const
{
    return m_gattCharacteristic->service()->objectPath();
}

QStringList GattCharacteristicAdaptor::flags() const
{
    // TODO: implement flags
    return { QStringLiteral("read"), QStringLiteral("write") };
}

QByteArray GattCharacteristicAdaptor::ReadValue(const QVariantMap &/*options*/)
{
    return m_gattCharacteristic->readValue();
}

void GattCharacteristicAdaptor::WriteValue(const QByteArray &value, const QVariantMap &/*options*/)
{
    m_gattCharacteristic->writeValue(value);
}

void GattCharacteristicAdaptor::StartNotify()
{
    // TODO: implement
}

void GattCharacteristicAdaptor::StopNotify()
{
    // TODO: implement
}

} // namespace BluezQt
