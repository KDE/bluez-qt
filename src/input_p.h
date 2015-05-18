/*
 * BluezQt - Asynchronous Bluez wrapper library
 *
 * Copyright (C) 2015 David Rosca <nowrep@gmail.com>
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

#ifndef BLUEZQT_INPUT_P_H
#define BLUEZQT_INPUT_P_H

#include <QObject>

#include "input.h"
#include "dbusproperties.h"

namespace BluezQt
{

typedef org::freedesktop::DBus::Properties DBusProperties;

class InputPrivate : public QObject
{
    Q_OBJECT

public:
    explicit InputPrivate(const QString &path, const QVariantMap &properties);

    void propertiesChanged(const QString &interface, const QVariantMap &changed, const QStringList &invalidated);

    QWeakPointer<Input> q;
    DBusProperties *m_dbusProperties;

    Input::ReconnectMode m_reconnectMode;
};

} // namespace BluezQt

#endif // BLUEZQT_INPUT_P_H
