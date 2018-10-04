/*
 * BluezQt - Asynchronous Bluez wrapper library
 *
 * Copyright (C) 2018 Manuel Weichselbaumer <mincequi@web.de>
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

#ifndef BLUEZQT_MEDIAENDPOINTADAPTOR_H
#define BLUEZQT_MEDIAENDPOINTADAPTOR_H

#include <QDBusAbstractAdaptor>

class QDBusMessage;
class QDBusObjectPath;

namespace BluezQt
{

class MediaEndpoint;

class MediaEndpointAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.bluez.MediaEndpoint1")

public:
    explicit MediaEndpointAdaptor(MediaEndpoint *parent);

public Q_SLOTS:
    void SetConfiguration(const QDBusObjectPath &transport, const QVariantMap &properties);
    QByteArray SelectConfiguration(const QByteArray &capabilities, const QDBusMessage &msg);
    void ClearConfiguration(const QDBusObjectPath &transport);
    Q_NOREPLY void Release();

private:
    MediaEndpoint *m_endpoint;
};

} // namespace BluezQt

#endif // BLUEZQT_MEDIAENDPOINTADAPTOR_H
