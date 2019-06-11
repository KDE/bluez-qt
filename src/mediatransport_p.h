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

#pragma once

#include "mediatransport.h"
#include "dbusproperties.h"

namespace BluezQt
{

typedef org::freedesktop::DBus::Properties DBusProperties;

class MediaTransportPrivate : public QObject
{
    Q_OBJECT

public:
    explicit MediaTransportPrivate(const QString &path, const QVariantMap &properties);

    void init(const QVariantMap &properties);

public Q_SLOTS:
    void onPropertiesChanged(const QString &interface, const QVariantMap &changed, const QStringList &invalidated);

public:
    QWeakPointer<MediaTransport> q;
    QDBusInterface *m_dbusInterface;
    DBusProperties *m_dbusProperties;

    QString m_path;
    AudioConfiguration m_configuration;
    MediaTransport::State m_state = MediaTransport::State::Idle;
    quint16 m_volume = 0;
};

} // namespace BluezQt
