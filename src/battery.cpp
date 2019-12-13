/*
 * BluezQt - Asynchronous Bluez wrapper library
 *
 * Copyright (C) 2019 Kai Uwe Broulik <kde@broulik.de>
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

#include "battery.h"
#include "battery_p.h"
#include "utils.h"
#include "macros.h"

#include <QVariantMap>

namespace BluezQt
{

BatteryPrivate::BatteryPrivate(const QString &path, const QVariantMap &properties)
    : QObject()
    , m_path(path)
{
    // Init properties
    m_percentage = properties.value(QStringLiteral("Percentage")).toInt();
}

void BatteryPrivate::propertiesChanged(const QString &interface, const QVariantMap &changed, const QStringList &invalidated)
{
    Q_UNUSED(invalidated)

    if (interface != Strings::orgBluezBattery1()) {
        return;
    }

    QVariantMap::const_iterator i;
    for (i = changed.constBegin(); i != changed.constEnd(); ++i) {
        const QVariant &value = i.value();
        const QString &property = i.key();

        if (property == QLatin1String("Percentage")) {
            PROPERTY_CHANGED2(m_percentage, value.toInt(), percentageChanged)
        }
    }
}

Battery::Battery(const QString &path, const QVariantMap &properties)
    : d(new BatteryPrivate(path, properties))
{
}

Battery::~Battery()
{
    delete d;
}

BatteryPtr Battery::toSharedPtr() const
{
    return d->q.toStrongRef();
}

int Battery::percentage() const
{
    return d->m_percentage;
}

} // namespace BluezQt
