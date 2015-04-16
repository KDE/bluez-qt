/*
 * Copyright (C) 2014-2015 David Rosca <nowrep@gmail.com>
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

#ifndef OBEXCLIENT_H
#define OBEXCLIENT_H

#include "object.h"

#include <QDBusAbstractAdaptor>

class QDBusMessage;

class ObexClient : public QDBusAbstractAdaptor, public Object
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.bluez.obex.Client1")

public:
    explicit ObexClient(QObject *parent = Q_NULLPTR);

    void runAction(const QString &actionName, const QVariantMap &properties);

public Q_SLOTS:
    void CreateSession(const QString &destination, const QVariantMap &args, const QDBusMessage &msg);
    void RemoveSession(const QDBusObjectPath &session, const QDBusMessage &msg);
};

#endif // OBEXCLIENT_H
