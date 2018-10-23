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

#include "mediaendpointconnector.h"

#include <QCoreApplication>
#include <QDBusObjectPath>
#include <QDebug>

#include "device.h"
#include "initmanagerjob.h"
#include "media.h"
#include "mediaendpoint.h"
#include "services.h"

using namespace BluezQt;

class NoInputNoOutputAgentPrivate
{
public:
    QStringList allowedUuids;
};

NoInputNoOutputAgent::NoInputNoOutputAgent(const QStringList &uuids, QObject *parent)
    : Agent(parent)
    , d(new NoInputNoOutputAgentPrivate)
{
    d->allowedUuids = uuids;
}

NoInputNoOutputAgent::~NoInputNoOutputAgent()
{
    delete d;
}

QDBusObjectPath NoInputNoOutputAgent::objectPath() const
{
    return QDBusObjectPath("/Agent/NoInputNoOutput");
}

Agent::Capability NoInputNoOutputAgent::capability() const
{
    return Agent::NoInputNoOutput;
}

void NoInputNoOutputAgent::authorizeService(DevicePtr device, const QString &uuid, const Request<> &request)
{
    Q_UNUSED(device)

    d->allowedUuids.contains(uuid) ? request.accept() : request.reject();
    emit serviceAuthorized(device, uuid, d->allowedUuids.contains(uuid));
}

MediaEndpointConnector::MediaEndpointConnector(Manager *manager, QObject *parent)
    : QObject(parent)
    , m_manager(manager)
{
    NoInputNoOutputAgent *agent = new NoInputNoOutputAgent({Services::AdvancedAudioDistribution, Services::AudioVideoRemoteControl});
    connect(agent, &NoInputNoOutputAgent::serviceAuthorized, this, &MediaEndpointConnector::onServiceAuthorized);
    manager->registerAgent(agent);
    manager->requestDefaultAgent(agent);

    MediaEndpoint *sbcSink = new MediaEndpoint({MediaEndpoint::Role::AudioSink, MediaEndpoint::Codec::Sbc}, manager);
    MediaEndpoint *aacSink = new MediaEndpoint({MediaEndpoint::Role::AudioSink, MediaEndpoint::Codec::Aac}, manager);
    connect(sbcSink, &MediaEndpoint::configurationSelected, this, &MediaEndpointConnector::onConfigurationSelected);
    connect(aacSink, &MediaEndpoint::configurationSelected, this, &MediaEndpointConnector::onConfigurationSelected);
    connect(sbcSink, &MediaEndpoint::configurationSet, this, &MediaEndpointConnector::onConfigurationSet);
    connect(aacSink, &MediaEndpoint::configurationSet, this, &MediaEndpointConnector::onConfigurationSet);
    connect(sbcSink, &MediaEndpoint::configurationCleared, this, &MediaEndpointConnector::onConfigurationCleared);
    connect(aacSink, &MediaEndpoint::configurationCleared, this, &MediaEndpointConnector::onConfigurationCleared);
    manager->media()->registerEndpoint(sbcSink);
    manager->media()->registerEndpoint(aacSink);
}

void MediaEndpointConnector::onServiceAuthorized(BluezQt::DevicePtr device, const QString &uuid, bool allowed)
{
    qDebug() << (allowed ? "Accepted" : "Rejected") << "service:" << uuid << "from" << device->friendlyName();
}

void MediaEndpointConnector::onConfigurationSelected(const QByteArray &capabilities, const QByteArray &configuration)
{
    if (configuration.isEmpty()) {
        qDebug() << "No useable configuration found for capabilities:" << capabilities;
    } else {
        qDebug() << "Selected configuration:" << configuration << "for capabilities:" << capabilities;
    }
}

void MediaEndpointConnector::onConfigurationSet(const QString &transportObjectPath, const QVariantMap &properties)
{
    qDebug() << "Set configuration for transport:" << transportObjectPath << "to:" << properties;
}

void MediaEndpointConnector::onConfigurationCleared(const QString &transportObjectPath)
{
    qDebug() << "Cleared configuration for transport:" << transportObjectPath;
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    qDebug() << "Waiting for bluetooth audio source to connect. Ctrl + C to cancel...";

    Manager *manager = new Manager();
    InitManagerJob *initJob = manager->init();
    initJob->exec();
    if (initJob->error()) {
        qWarning() << "Error initializing manager:" << initJob->errorText();
        return 1;
    }

    new MediaEndpointConnector(manager);

    return app.exec();
}
