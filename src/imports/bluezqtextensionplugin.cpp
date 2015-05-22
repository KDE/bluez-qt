/*
 * BluezQt - Asynchronous BlueZ wrapper library
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

#include "bluezqtextensionplugin.h"
#include "declarativemanager.h"
#include "declarativeadapter.h"
#include "declarativedevice.h"
#include "declarativeinput.h"
#include "declarativemediaplayer.h"
#include "declarativedevicesmodel.h"
#include "device.h"
#include "pendingcall.h"
#include "services.h"

#include <QtQml> // krazy:exclude=includes

static QObject *manager_singleton(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return new DeclarativeManager;
}

static QJSValue services_singleton(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)

    QJSValue object = scriptEngine->newObject();
    object.setProperty(QStringLiteral("ServiceDiscoveryServer"), BluezQt::Services::ServiceDiscoveryServer);
    object.setProperty(QStringLiteral("SerialPort"), BluezQt::Services::SerialPort);
    object.setProperty(QStringLiteral("DialupNetworking"), BluezQt::Services::DialupNetworking);
    object.setProperty(QStringLiteral("ObexObjectPush"), BluezQt::Services::ObexObjectPush);
    object.setProperty(QStringLiteral("ObexFileTransfer"), BluezQt::Services::ObexFileTransfer);
    object.setProperty(QStringLiteral("Headset"), BluezQt::Services::Headset);
    object.setProperty(QStringLiteral("AudioSource"), BluezQt::Services::AudioSource);
    object.setProperty(QStringLiteral("AudioVideoRemoteControlTarget"), BluezQt::Services::AudioVideoRemoteControlTarget);
    object.setProperty(QStringLiteral("AdvancedAudioDistribution"), BluezQt::Services::AdvancedAudioDistribution);
    object.setProperty(QStringLiteral("AudioVideoRemoteControl"), BluezQt::Services::AudioVideoRemoteControl);
    object.setProperty(QStringLiteral("HeadsetAudioGateway"), BluezQt::Services::HeadsetAudioGateway);
    object.setProperty(QStringLiteral("Panu"), BluezQt::Services::Panu);
    object.setProperty(QStringLiteral("Nap"), BluezQt::Services::Nap);
    object.setProperty(QStringLiteral("Handsfree"), BluezQt::Services::Handsfree);
    object.setProperty(QStringLiteral("HandsfreeAudioGateway"), BluezQt::Services::HandsfreeAudioGateway);
    object.setProperty(QStringLiteral("HumanInterfaceDevice"), BluezQt::Services::HumanInterfaceDevice);
    object.setProperty(QStringLiteral("SimAccess"), BluezQt::Services::SimAccess);
    object.setProperty(QStringLiteral("PhonebookAccessServer"), BluezQt::Services::PhonebookAccessServer);
    object.setProperty(QStringLiteral("MessageAccessServer"), BluezQt::Services::MessageAccessServer);
    object.setProperty(QStringLiteral("PnpInformation"), BluezQt::Services::PnpInformation);
    return object;
}

void BluezQtExtensionPlugin::registerTypes(const char *uri)
{
    using namespace BluezQt;

    Q_ASSERT(QLatin1String(uri) == QLatin1String("org.kde.bluezqt"));

    qmlRegisterSingletonType<DeclarativeManager>(uri, 1, 0, "Manager", manager_singleton);
    qmlRegisterType<DeclarativeDevicesModel>(uri, 1, 0, "DevicesModelPrivate");
    qmlRegisterUncreatableType<DeclarativeAdapter>(uri, 1, 0, "Adapter", QStringLiteral("Adapter cannot be created"));
    qmlRegisterUncreatableType<DeclarativeDevice>(uri, 1, 0, "Device", QStringLiteral("Device cannot be created"));
    qmlRegisterUncreatableType<DeclarativeInput>(uri, 1, 0, "Input", QStringLiteral("Input cannot be created"));
    qmlRegisterUncreatableType<DeclarativeMediaPlayer>(uri, 1, 0, "MediaPlayer", QStringLiteral("MediaPlayer cannot be created"));
    qmlRegisterUncreatableType<PendingCall>(uri, 1, 0, "PendingCall", QStringLiteral("PendingCall cannot be created"));
    qmlRegisterSingletonType(uri, 1, 0, "Services", services_singleton);
}
