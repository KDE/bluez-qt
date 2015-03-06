/*
 * BluezQt - Asynchronous Bluez wrapper library
 *
 * Copyright (C) 2014 David Rosca <nowrep@gmail.com>
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

#include "profileadaptor.h"
#include "profile.h"
#include "manager.h"
#include "device.h"

#include <QDBusMessage>
#include <QDBusObjectPath>
#include <QDBusUnixFileDescriptor>

namespace BluezQt
{

ProfileAdaptor::ProfileAdaptor(Profile *parent, Manager *manager)
    : QDBusAbstractAdaptor(parent)
    , m_profile(parent)
    , m_manager(manager)
{
}

void ProfileAdaptor::NewConnection(const QDBusObjectPath &device, const QDBusUnixFileDescriptor &fd, const QVariantMap &properties, const QDBusMessage &msg)
{
    msg.setDelayedReply(true);
    Request<> req(OrgBluezProfile, msg);

    DevicePtr dev = m_manager->deviceForUbi(device.path());
    if (!dev) {
        req.cancel();
        return;
    }

    m_profile->newConnection(dev, fd, properties, req);
}

void ProfileAdaptor::RequestDisconnection(const QDBusObjectPath &device, const QDBusMessage &msg)
{
    msg.setDelayedReply(true);
    Request<> req(OrgBluezProfile, msg);

    DevicePtr dev = m_manager->deviceForUbi(device.path());
    if (!dev) {
        req.cancel();
        return;
    }

    m_profile->requestDisconnection(dev, req);
}

void ProfileAdaptor::Release()
{
    m_profile->release();
}

} // namespace BluezQt
