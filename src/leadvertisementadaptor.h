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

#ifndef BLUEZQT_LEADVERTISEMENTADAPTOR_H
#define BLUEZQT_LEADVERTISEMENTADAPTOR_H

#include <QDBusAbstractAdaptor>

namespace BluezQt
{

class LEAdvertisement;

class LEAdvertisementAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.bluez.LEAdvertisement1")
    Q_PROPERTY(QString Type READ type)
    Q_PROPERTY(QStringList ServiceUUIDs READ serviceUuids)

public:
    explicit LEAdvertisementAdaptor(LEAdvertisement *parent);

    QString type() const;

    QStringList serviceUuids() const;

public Q_SLOTS:
    Q_NOREPLY void Release();

private:
    LEAdvertisement *m_advertisement;
};

} // namespace BluezQt

#endif // BLUEZQT_LEADVERTISEMENTADAPTOR_H
