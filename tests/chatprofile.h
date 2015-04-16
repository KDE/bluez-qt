/*
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

#ifndef CHATPROFILE_H
#define CHATPROFILE_H

#include "profile.h"
#include "device.h"

class QLocalSocket;

class ChatProfile : public BluezQt::Profile
{
    Q_OBJECT

public:
    explicit ChatProfile(QObject *parent);

    QDBusObjectPath objectPath() const;
    QString uuid() const;

    void newConnection(BluezQt::DevicePtr device, const QDBusUnixFileDescriptor &fd, const QVariantMap &properties, const BluezQt::Request<> &request);
    void requestDisconnection(BluezQt::DevicePtr device, const BluezQt::Request<> &request);
    void release();

private Q_SLOTS:
    void socketReadyRead();
    void socketDisconnected();
    void writeToSocket();

private:
    QSharedPointer<QLocalSocket> m_socket;
};

#endif // CHATPROFILE_H
