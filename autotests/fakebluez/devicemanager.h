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

#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QObject>

class ObjectManager;

class DeviceManager : public QObject
{
    Q_OBJECT

public:
    explicit DeviceManager(ObjectManager *parent = Q_NULLPTR);

    void runAction(const QString &actionName, const QVariantMap &properties);

private:
    void runCreateAdapterAction(const QVariantMap &properties);
    void runCreateDeviceAction(const QVariantMap &properties);
    void runRemoveAdapterAction(const QVariantMap &properties);
    void runRemoveDeviceAction(const QVariantMap &properties);
    void runChangeAdapterProperty(const QVariantMap &properties);
    void runChangeDeviceProperty(const QVariantMap &properties);

    ObjectManager *m_objectManager;

};

#endif // DEVICEMANAGER_H
