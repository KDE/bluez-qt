/*
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

#pragma once

#include "object.h"

#include <QDBusAbstractAdaptor>

class QDBusMessage;

class MediaInterface : public QDBusAbstractAdaptor, public Object
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.bluez.Media1")

public:
    explicit MediaInterface(const QDBusObjectPath &path, QObject *parent = nullptr);

    void runAction(const QString &actionName, const QVariantMap &properties);

public Q_SLOTS:
    void RegisterEndpoint(const QDBusObjectPath &path, const QVariantMap &properties, const QDBusMessage &msg);
    void UnregisterEndpoint(const QDBusObjectPath &path, const QDBusMessage &msg);

private:
    void runSetConfigurationAction(const QVariantMap &properties);
    void runSelectConfigurationAction(const QVariantMap &properties);
    void runClearConfigurationAction(const QVariantMap &properties);
    void runReleaseAction();

    QDBusObjectPath m_endpoint;
    QString m_service;
    QVariantMap m_properties;
};
