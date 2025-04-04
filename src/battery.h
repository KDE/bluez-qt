/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * SPDX-FileCopyrightText: 2019 Kai Uwe Broulik <kde@broulik.de>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef BLUEZQT_BATTERY_H
#define BLUEZQT_BATTERY_H

#include <QObject>

#include "bluezqt_export.h"
#include "types.h"

#include <memory>

namespace BluezQt
{
/*!
 * \inmodule BluezQt
 * \class BluezQt::Battery
 * \inheaderfile BluezQt/Battery
 *
 * \brief Device battery.
 *
 * This class represents a battery interface.
 */
class BLUEZQT_EXPORT Battery : public QObject
{
    Q_OBJECT
    /*!
     * \property BluezQt::Battery::percentage
     */
    Q_PROPERTY(int percentage READ percentage NOTIFY percentageChanged)

public:
    ~Battery() override;

    /*!
     * Returns a shared pointer from this.
     */
    BatteryPtr toSharedPtr() const;

    /*!
     * Returns the battery percentage.
     */
    int percentage() const;

Q_SIGNALS:
    /*!
     * Indicates that battery's \a percentage has changed.
     */
    void percentageChanged(int percentage);

private:
    BLUEZQT_NO_EXPORT explicit Battery(const QString &path, const QVariantMap &properties);

    std::unique_ptr<class BatteryPrivate> const d;

    friend class BatteryPrivate;
    friend class DevicePrivate;
};

} // namespace BluezQt

#endif // BLUEZQT_BATTERY_H
