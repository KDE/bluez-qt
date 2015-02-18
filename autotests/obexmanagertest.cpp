#include "obexmanagertest.h"
#include "autotests.h"
#include "obexmanager.h"
#include "initobexmanagerjob.h"

#include <QtTest/QTest>
#include <QtTest/QSignalSpy>
#include <QLoggingCategory>

namespace BluezQt
{
extern void bluezqt_initFakeBluezTestRun();
}

using namespace BluezQt;

void ObexManagerTest::initTestCase()
{
    bluezqt_initFakeBluezTestRun();
    QLoggingCategory::setFilterRules(QStringLiteral("BluezQt=false"));
}

void ObexManagerTest::cleanupTestCase()
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
}

QTEST_MAIN(ObexManagerTest)
