/*
 * BluezQt - Asynchronous Bluez wrapper library
 *
 * SPDX-FileCopyrightText: 2019 Manuel Weichselbaumer
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "gattcharacteristicadaptor.h"
#include "gattcharacteristic.h"
#include "gattservice.h"

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
    return {QStringLiteral("read"), QStringLiteral("write")};
}

QByteArray GattCharacteristicAdaptor::ReadValue(const QVariantMap & /*options*/)
{
    return m_gattCharacteristic->readValue();
}

void GattCharacteristicAdaptor::WriteValue(const QByteArray &value, const QVariantMap & /*options*/)
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
