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

#include "input.h"
#include "input_p.h"
#include "utils.h"
#include "macros.h"

namespace BluezQt
{

typedef org::freedesktop::DBus::Properties DBusProperties;

static Input::ReconnectMode stringToReconnectMode(const QString &mode)
{
    if (mode == QLatin1String("none")) {
        return Input::NoReconnect;
    } else if (mode == QLatin1String("host")) {
        return Input::HostReconnect;
    } else if (mode == QLatin1String("device")) {
        return Input::DeviceReconnect;
    }
    return Input::AnyReconnect;
}

InputPrivate::InputPrivate(const QString &path, const QVariantMap &properties)
    : QObject()
{
    m_dbusProperties = new DBusProperties(Strings::orgBluez(), path,
                                          DBusConnection::orgBluez(), this);

    connect(m_dbusProperties, &DBusProperties::PropertiesChanged,
            this, &InputPrivate::propertiesChanged, Qt::QueuedConnection);

    // Init properties
    m_reconnectMode = stringToReconnectMode(properties.value(QStringLiteral("ReconnectMode")).toString());
}

void InputPrivate::propertiesChanged(const QString &interface, const QVariantMap &changed, const QStringList &invalidated)
{
    Q_UNUSED(invalidated)

    if (interface != Strings::orgBluezInput1()) {
        return;
    }

    QVariantMap::const_iterator i;
    for (i = changed.constBegin(); i != changed.constEnd(); ++i) {
        const QVariant &value = i.value();
        const QString &property = i.key();

        if (property == QLatin1String("ReconnectMode")) {
            PROPERTY_CHANGED2(m_reconnectMode, stringToReconnectMode(value.toString()), reconnectModeChanged);
        }
    }
}

Input::Input(const QString &path, const QVariantMap &properties)
    : d(new InputPrivate(path, properties))
{
}

Input::~Input()
{
    delete d;
}

InputPtr Input::toSharedPtr() const
{
    return d->q.toStrongRef();
}

Input::ReconnectMode Input::reconnectMode() const
{
    return d->m_reconnectMode;
}

} // namespace BluezQt
