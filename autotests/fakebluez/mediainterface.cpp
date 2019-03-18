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

#include "mediainterface.h"

#include <QDebug>
#include <QDBusMessage>
#include <QDBusConnection>
#include <QDBusPendingCall>

MediaInterface::MediaInterface(const QDBusObjectPath &path, QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    setName(QStringLiteral("org.bluez.Media1"));
    setPath(path);
}

void MediaInterface::runAction(const QString &actionName, const QVariantMap &properties)
{
    if (actionName == QLatin1String("set-configuration")) {
        runSetConfigurationAction(properties);
    } else if (actionName == QLatin1String("select-configuration")) {
        runSelectConfigurationAction(properties);
    } else if (actionName == QLatin1String("clear-configuration")) {
        runClearConfigurationAction(properties);
    } else if (actionName == QLatin1String("release")) {
        runReleaseAction();
    }
}

void MediaInterface::RegisterEndpoint(const QDBusObjectPath &path, const QVariantMap &properties, const QDBusMessage &msg)
{
    m_endpoint = path;
    m_service = msg.service();
    m_properties = properties;
}

void MediaInterface::UnregisterEndpoint(const QDBusObjectPath &path, const QDBusMessage &msg)
{
    if (m_endpoint == path && m_service == msg.service()) {
        m_endpoint = QDBusObjectPath();
        m_service.clear();
        m_properties.clear();
    }
}

void MediaInterface::runSetConfigurationAction(const QVariantMap &properties)
{
    QDBusMessage call = QDBusMessage::createMethodCall(m_service,
                                                       m_endpoint.path(),
                                                       QStringLiteral("org.bluez.MediaEndpoint1"),
                                                       QStringLiteral("SetConfiguration"));
    call << QVariant::fromValue(properties.value(QStringLiteral("Transport")).value<QDBusObjectPath>());
    call << properties.value(QStringLiteral("Properties"));
    QDBusConnection::sessionBus().asyncCall(call);
}

void MediaInterface::runSelectConfigurationAction(const QVariantMap &properties)
{
    QDBusMessage call = QDBusMessage::createMethodCall(m_service,
                                                       m_endpoint.path(),
                                                       QStringLiteral("org.bluez.MediaEndpoint1"),
                                                       QStringLiteral("SelectConfiguration"));
    call << properties.value(QStringLiteral("Capabilities"));
    QDBusConnection::sessionBus().asyncCall(call);
}

void MediaInterface::runClearConfigurationAction(const QVariantMap &properties)
{
    QDBusMessage call = QDBusMessage::createMethodCall(m_service,
                                                       m_endpoint.path(),
                                                       QStringLiteral("org.bluez.MediaEndpoint1"),
                                                       QStringLiteral("ClearConfiguration"));
    call << QVariant::fromValue(properties.value(QStringLiteral("Transport")).value<QDBusObjectPath>());
    QDBusConnection::sessionBus().asyncCall(call);
}

void MediaInterface::runReleaseAction()
{
    QDBusMessage call = QDBusMessage::createMethodCall(m_service,
                                                       m_endpoint.path(),
                                                       QStringLiteral("org.bluez.MediaEndpoint1"),
                                                       QStringLiteral("Release"));
    QDBusConnection::sessionBus().asyncCall(call);
}
