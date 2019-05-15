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

#include "leadvertisingmanagerinterface.h"

#include <QDebug>
#include <QDBusMessage>
#include <QDBusConnection>
#include <QDBusPendingCall>

LEAdvertisingManagerInterface::LEAdvertisingManagerInterface(const QDBusObjectPath &path, QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    setName(QStringLiteral("org.bluez.LEAdvertisingManager1"));
    setPath(path);
}

void LEAdvertisingManagerInterface::runAction(const QString &actionName, const QVariantMap &/*properties*/)
{
    if (actionName == QLatin1String("release")) {
        runReleaseAction();
    }
}

void LEAdvertisingManagerInterface::RegisterAdvertisement(const QDBusObjectPath &path, const QVariantMap &/*options*/, const QDBusMessage &msg)
{
    m_advertisement = path;
    m_service = msg.service();
}

void LEAdvertisingManagerInterface::UnregisterAdvertisement(const QDBusObjectPath &path, const QDBusMessage &msg)
{
    if (m_advertisement == path && m_service == msg.service()) {
        m_advertisement = QDBusObjectPath();
        m_service.clear();
    }
}

void LEAdvertisingManagerInterface::runReleaseAction()
{
    QDBusMessage call = QDBusMessage::createMethodCall(m_service,
                                                       m_advertisement.path(),
                                                       QStringLiteral("org.bluez.LEAdvertisement1"),
                                                       QStringLiteral("Release"));
    QDBusConnection::sessionBus().asyncCall(call);
}
