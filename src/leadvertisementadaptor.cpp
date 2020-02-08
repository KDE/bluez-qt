/*
 * BluezQt - Asynchronous Bluez wrapper library
 *
 * SPDX-FileCopyrightText: 2019 Manuel Weichselbaumer <mincequi@web.de>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "leadvertisementadaptor.h"
#include "leadvertisement.h"

#include <QDBusObjectPath>

namespace BluezQt
{

LEAdvertisementAdaptor::LEAdvertisementAdaptor(LEAdvertisement *parent)
    : QDBusAbstractAdaptor(parent)
    , m_advertisement(parent)
{
}

QString LEAdvertisementAdaptor::type() const
{
    return QStringLiteral("peripheral");
}

QStringList LEAdvertisementAdaptor::serviceUuids() const
{
    return m_advertisement->serviceUuids();
}

void LEAdvertisementAdaptor::Release()
{
    m_advertisement->release();
}

} // namespace BluezQt
