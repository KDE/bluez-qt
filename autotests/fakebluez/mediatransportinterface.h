/*
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

#include "object.h"

#include <QDBusAbstractAdaptor>

class QDBusMessage;

class MediaTransportObject : public QObject
{
public:
    explicit MediaTransportObject(const QDBusObjectPath &path, QObject *parent = nullptr);
};

class MediaTransportInterface : public QDBusAbstractAdaptor, public Object
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.bluez.MediaTransport1")
    Q_PROPERTY(quint8 Codec READ codec)
    Q_PROPERTY(QByteArray Configuration READ configuration)
    Q_PROPERTY(QString State READ state)
    Q_PROPERTY(quint16 Volume READ volume)

public:
    explicit MediaTransportInterface(const QDBusObjectPath &path, const QVariantMap &properties, QObject *parent = nullptr);

    quint8 codec() const;
    QByteArray configuration() const;
    QString state() const;
    quint16 volume() const;

public Q_SLOTS:
    void Acquire(const QDBusMessage &msg);
    void TryAcquire(const QDBusMessage &msg);
    void Release();
};
