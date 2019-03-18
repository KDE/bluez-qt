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

#include "mediatest.h"
#include "a2dp-codecs.h"
#include "autotests.h"
#include "initmanagerjob.h"
#include "manager.h"
#include "media.h"
#include "mediaendpoint.h"
#include "pendingcall.h"

#include <QDebug>
#include <QSignalSpy>
#include <QDBusObjectPath>

namespace BluezQt
{
extern void bluezqt_initFakeBluezTestRun();
}

using namespace BluezQt;

void TestEndpoint::release()
{
    m_releaseCalled = true;
}

void MediaTest::initTestCase()
{
    bluezqt_initFakeBluezTestRun();
    FakeBluez::start();
    FakeBluez::runTest(QStringLiteral("bluez-standard"));

    // Create adapter
    QString adapter = QStringLiteral("/org/bluez/hci0");
    QVariantMap adapterProps;
    adapterProps[QStringLiteral("Path")] = QVariant::fromValue(QDBusObjectPath(adapter));
    adapterProps[QStringLiteral("Address")] = QStringLiteral("1C:E5:C3:BC:94:7E");
    adapterProps[QStringLiteral("Name")] = QStringLiteral("TestAdapter");
    FakeBluez::runAction(QStringLiteral("devicemanager"), QStringLiteral("create-adapter"), adapterProps);

    Manager *manager = new Manager(this);
    InitManagerJob *initJob = manager->init();
    initJob->exec();
    QVERIFY(!initJob->error());
    QCOMPARE(manager->adapters().count(), 1);

    m_adapter = manager->adapters().at(0);
    QVERIFY(m_adapter->media());

    m_endpoint = new TestEndpoint({MediaEndpoint::Role::AudioSink, MediaEndpoint::Codec::Sbc}, this);
    m_adapter->media()->registerEndpoint(m_endpoint)->waitForFinished();
}

void MediaTest::cleanupTestCase()
{
    FakeBluez::stop();
}

void MediaTest::setConfigurationTest()
{
    QSignalSpy endpointSpy(m_endpoint, SIGNAL(configurationSet(QString,QVariantMap)));

    QVariantMap props;
    props.insert(QStringLiteral("Key"), QVariant::fromValue(int(123)));

    QVariantMap params;
    params.insert(QStringLiteral("AdapterPath"), QVariant::fromValue(QDBusObjectPath(m_adapter->ubi())));
    params.insert(QStringLiteral("Transport"), QVariant::fromValue(m_endpoint->objectPath()));
    params.insert(QStringLiteral("Properties"), props);
    FakeBluez::runAction(QStringLiteral("devicemanager"), QStringLiteral("adapter-media:set-configuration"), params);
    endpointSpy.wait();
    auto args = endpointSpy.takeFirst();
    QCOMPARE(args.at(0).toString(), m_endpoint->objectPath().path());
    QCOMPARE(args.at(1).value<QVariantMap>(), props);
}

void MediaTest::selectConfigurationTest()
{
    QSignalSpy endpointSpy(m_endpoint, SIGNAL(configurationSelected(QByteArray,QByteArray)));

    a2dp_sbc_t sbcConfiguration;
    sbcConfiguration.frequency = SBC_SAMPLING_FREQ_44100;
    sbcConfiguration.channel_mode = SBC_CHANNEL_MODE_STEREO;
    sbcConfiguration.block_length = SBC_BLOCK_LENGTH_16;
    sbcConfiguration.subbands = SBC_SUBBANDS_8;
    sbcConfiguration.allocation_method = SBC_ALLOCATION_LOUDNESS;
    sbcConfiguration.min_bitpool = 2;
    sbcConfiguration.max_bitpool = 53;

    QVariantMap params;
    params.insert(QStringLiteral("AdapterPath"), QVariant::fromValue(QDBusObjectPath(m_adapter->ubi())));
    params.insert(QStringLiteral("Capabilities"), QByteArray(reinterpret_cast<const char*>(&sbcCapabilities), sizeof(sbcCapabilities)));
    FakeBluez::runAction(QStringLiteral("devicemanager"), QStringLiteral("adapter-media:select-configuration"), params);
    endpointSpy.wait();
    auto args = endpointSpy.takeFirst();
    QCOMPARE(args.at(0).toByteArray(), QByteArray(reinterpret_cast<const char*>(&sbcCapabilities), sizeof(sbcCapabilities)));
    QCOMPARE(args.at(1).toByteArray(), QByteArray(reinterpret_cast<const char*>(&sbcConfiguration), sizeof(sbcConfiguration)));

    params.insert(QStringLiteral("Capabilities"), QByteArray());
    FakeBluez::runAction(QStringLiteral("devicemanager"), QStringLiteral("adapter-media:select-configuration"), params);
    endpointSpy.wait();
    args = endpointSpy.takeFirst();
    QCOMPARE(args.at(0).toByteArray(), QByteArray());
    QCOMPARE(args.at(1).toByteArray(), QByteArray());
}

void MediaTest::clearConfigurationTest()
{
    QSignalSpy endpointSpy(m_endpoint, SIGNAL(configurationCleared(QString)));

    QVariantMap params;
    params.insert(QStringLiteral("AdapterPath"), QVariant::fromValue(QDBusObjectPath(m_adapter->ubi())));
    params.insert(QStringLiteral("Transport"), QVariant::fromValue(m_endpoint->objectPath()));
    FakeBluez::runAction(QStringLiteral("devicemanager"), QStringLiteral("adapter-media:clear-configuration"), params);
    endpointSpy.wait();
    auto args = endpointSpy.takeFirst();
    QCOMPARE(args.at(0).toString(), m_endpoint->objectPath().path());
}

void MediaTest::releaseTest()
{
    QCOMPARE(m_endpoint->m_releaseCalled, false);

    QVariantMap params;
    params.insert(QStringLiteral("AdapterPath"), QVariant::fromValue(QDBusObjectPath(m_adapter->ubi())));
    FakeBluez::runAction(QStringLiteral("devicemanager"), QStringLiteral("adapter-media:release"), params);

    QTRY_COMPARE(m_endpoint->m_releaseCalled, true);
}

QTEST_MAIN(MediaTest)
