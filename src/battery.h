/*
 * BluezQt - Asynchronous BlueZ wrapper library
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

#ifndef BLUEZQT_BATTERY_H
#define BLUEZQT_BATTERY_H

#include <QObject>

#include "types.h"
#include "bluezqt_export.h"

namespace BluezQt
{

/**
 * @class BluezQt::Battery battery.h <BluezQt/Battery>
 *
 * %Device battery.
 *
 * This class represents a battery interface.
 */
class BLUEZQT_EXPORT Battery : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int percentage READ percentage NOTIFY percentageChanged)

public:
    /**
     * Destroys a Battery object.
     */
    ~Battery();

    /**
     * Returns a shared pointer from this.
     *
     * @return BatteryPtr
     */
    BatteryPtr toSharedPtr() const;

    /**
     * Returns the battery percentage.
     *
     * @return battery percentage
     */
    int percentage() const;

Q_SIGNALS:
    /**
     * Indicates that battery's percentage has changed.
     */
    void percentageChanged(int percentage);

private:
    explicit Battery(const QString &path, const QVariantMap &properties);

    class BatteryPrivate *const d;

    friend class BatteryPrivate;
    friend class DevicePrivate;
};

} // namespace BluezQt

#endif // BLUEZQT_BATTERY_H
