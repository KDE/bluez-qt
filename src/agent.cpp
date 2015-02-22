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

#include "agent.h"

namespace BluezQt
{

Agent::Agent(QObject *parent)
    : QObject(parent)
{
}

Agent::Capability Agent::capability() const
{
    return DisplayYesNo;
}

void Agent::requestPinCode(DevicePtr device, const Request<QString> &request)
{
    Q_UNUSED(device)

    request.cancel();
}

void Agent::displayPinCode(DevicePtr device, const QString &pinCode)
{
    Q_UNUSED(device)
    Q_UNUSED(pinCode)
}

void Agent::requestPasskey(DevicePtr device, const Request<quint32> &request)
{
    Q_UNUSED(device)

    request.cancel();
}

void Agent::displayPasskey(DevicePtr device, const QString &passkey, const QString &entered)
{
    Q_UNUSED(device)
    Q_UNUSED(passkey)
    Q_UNUSED(entered)
}

void Agent::requestConfirmation(DevicePtr device, const QString &passkey, const Request<> &request)
{
    Q_UNUSED(device)
    Q_UNUSED(passkey)

    request.cancel();
}

void Agent::requestAuthorization(DevicePtr device, const Request<> &request)
{
    Q_UNUSED(device)

    request.cancel();
}

void Agent::authorizeService(DevicePtr device, const QString &uuid, const Request<> &request)
{
    Q_UNUSED(device)
    Q_UNUSED(uuid)

    request.cancel();
}

void Agent::cancel()
{
}

void Agent::release()
{
}

} // namespace BluezQt
