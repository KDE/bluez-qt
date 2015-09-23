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

#include "obexmanagertest.h"
#include "autotests.h"
#include "obexmanager.h"
#include "initobexmanagerjob.h"

#include <QtTest/QTest>
#include <QtTest/QSignalSpy>

namespace BluezQt
{
extern void bluezqt_initFakeBluezTestRun();
}

using namespace BluezQt;

void ObexManagerTest::initTestCase()
{
    bluezqt_initFakeBluezTestRun();
}

void ObexManagerTest::cleanup()
{
    FakeBluez::stop();
}

void ObexManagerTest::obexNotRunningTest()
{
    // org.bluez.obex is not running at all
    // expected: init successful
    ObexManager *manager = new ObexManager;
    InitObexManagerJob *job = manager->init();
    job->exec();

    QVERIFY(!job->error());
    QVERIFY(manager->isInitialized());
    QVERIFY(!manager->isOperational());

    delete manager;
}

void ObexManagerTest::obexNotExportingInterfacesTest()
{
    // org.bluez.obex is running, but it does not export any interfaces
    // expected: init error
    FakeBluez::start();
    FakeBluez::runTest(QStringLiteral("obex-not-exporting-interfaces"));

    ObexManager *manager = new ObexManager;
    InitObexManagerJob *job = manager->init();
    job->exec();

    QVERIFY(job->error());
    QVERIFY(!manager->isInitialized());
    QVERIFY(!manager->isOperational());

    delete manager;
}

void ObexManagerTest::obexEmptyManagedObjectsTest()
{
    // org.bluez.obex exports ObjectManager, but there is no AgentManager1 nor Client1
    // expected: init error
    FakeBluez::start();
    FakeBluez::runTest(QStringLiteral("obex-empty-managed-objects"));

    ObexManager *manager = new ObexManager;
    InitObexManagerJob *job = manager->init();
    job->exec();

    QVERIFY(job->error());
    QVERIFY(!manager->isInitialized());
    QVERIFY(!manager->isOperational());

    delete manager;
}

void ObexManagerTest::obexNoClientTest()
{
    // org.bluez.obex exports ObjectManager and AgentManager1, but there is no Client1
    // expected: init error
    FakeBluez::start();
    FakeBluez::runTest(QStringLiteral("obex-no-client"));

    ObexManager *manager = new ObexManager;
    InitObexManagerJob *job = manager->init();
    job->exec();

    QVERIFY(job->error());
    QVERIFY(!manager->isInitialized());
    QVERIFY(!manager->isOperational());

    delete manager;
}

void ObexManagerTest::obexNoAgentManagerTest()
{
    // org.bluez.obex exports ObjectManager and Client1, but there is no AgentManager1
    // expected: init error
    FakeBluez::start();
    FakeBluez::runTest(QStringLiteral("obex-no-agentmanager"));

    ObexManager *manager = new ObexManager;
    InitObexManagerJob *job = manager->init();
    job->exec();

    QVERIFY(job->error());
    QVERIFY(!manager->isInitialized());
    QVERIFY(!manager->isOperational());

    delete manager;
}

void ObexManagerTest::obexServiceOkTest()
{
    // org.bluez.obex is running and exports all used interfaces
    // expected: init successful
    FakeBluez::start();
    FakeBluez::runTest(QStringLiteral("obex-standard"));

    ObexManager *manager = new ObexManager;
    InitObexManagerJob *job = manager->init();
    job->exec();

    QVERIFY(!job->error());
    QVERIFY(manager->isInitialized());
    QVERIFY(manager->isOperational());

    delete manager;
}

QTEST_MAIN(ObexManagerTest)
