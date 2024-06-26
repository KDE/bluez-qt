/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * SPDX-FileCopyrightText: 2019 Kai Uwe Broulik <kde@broulik.de>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef DECLARATIVEBATTERY_H
#define DECLARATIVEBATTERY_H

#include "battery.h"
#include "qqmlregistration.h"

class DeclarativeBattery : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(Battery)
    QML_UNCREATABLE("Battery cannot be created")
    Q_PROPERTY(int percentage READ percentage NOTIFY percentageChanged)

public:
    explicit DeclarativeBattery(const BluezQt::BatteryPtr &battery, QObject *parent = nullptr);

    int percentage() const;

Q_SIGNALS:
    void percentageChanged(int percentage);

private:
    BluezQt::BatteryPtr m_battery;
};

#endif // DECLARATIVEBATTERY_H
