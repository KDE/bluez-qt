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

#ifndef MEDIAENDPOINTCONNECTOR_H
#define MEDIAENDPOINTCONNECTOR_H

#include <QObject>

#include "agent.h"
#include "manager.h"
#include "types.h"

class NoInputNoOutputAgent : public BluezQt::Agent
{
    Q_OBJECT

public:
    /**
     * Creates a new NoInputNoOutputAgent object.
     *
     * @param parent
     */
    explicit NoInputNoOutputAgent(const QStringList &uuids, QObject *parent = nullptr);

    /**
      * Destroys a NoInputNoOutputAgent object.
      */
    ~NoInputNoOutputAgent() override;

Q_SIGNALS:
    /**
     * Indicates that a service was authorized.
     */
    void serviceAuthorized(BluezQt::DevicePtr device, const QString &uuid, bool allowed);

private:
    QDBusObjectPath objectPath() const override;
    Capability capability() const override;
    void authorizeService(BluezQt::DevicePtr device, const QString &uuid, const BluezQt::Request<> &request) override;

    class NoInputNoOutputAgentPrivate *const d;
};

class MediaEndpointConnector : public QObject
{
    Q_OBJECT

public:
    explicit MediaEndpointConnector(BluezQt::Manager *manager, QObject *parent = nullptr);

private:
    void onTransportChanged(BluezQt::MediaTransportPtr transport);
    void onServiceAuthorized(BluezQt::DevicePtr device, const QString &uuid, bool allowed);
    void onConfigurationSelected(const QByteArray &capabilities, const QByteArray &configuration);
    void onConfigurationSet(const QString &transportObjectPath, const QVariantMap &properties);
    void onConfigurationCleared(const QString &transportObjectPath);

    BluezQt::Manager *m_manager;
};

#endif // MEDIAENDPOINTCONNECTOR_H
