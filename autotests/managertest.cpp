#include "managertest.h"
#include "autotests.h"
#include "manager.h"
#include "initmanagerjob.h"

#include <QtTest/QTest>
#include <QtTest/QSignalSpy>
#include <QLoggingCategory>

namespace QBluez
{
extern void qbluez_initFakeBluezTestRun();
}

using namespace QBluez;

void ManagerTest::initTestCase()
{
    qbluez_initFakeBluezTestRun();
    QLoggingCategory::setFilterRules(QStringLiteral("QBluez=false"));
}

void ManagerTest::cleanupTestCase()
{
    FakeBluez::stop();
}

void ManagerTest::bluezNotRunningTest()
{
    // org.bluez is not running at all
    // expected: init successful
    Manager *manager = new Manager;
    InitManagerJob *job = manager->init();
    job->exec();

    QVERIFY(!job->error());
    QVERIFY(manager->isInitialized());
    QVERIFY(!manager->isOperational());
    QVERIFY(!manager->isBluetoothOperational());
}

void ManagerTest::bluezNotExportingInterfacesTest()
{
    // org.bluez is running, but it does not export any interfaces
    // expected: init error
    FakeBluez::start();
    FakeBluez::runTest(QStringLiteral("bluez-not-exporting-interfaces"));

    Manager *manager = new Manager;
    InitManagerJob *job = manager->init();
    job->exec();

    QVERIFY(job->error());
    QVERIFY(!manager->isInitialized());
    QVERIFY(!manager->isOperational());
    QVERIFY(!manager->isBluetoothOperational());
}

void ManagerTest::bluezEmptyManagedObjectsTest()
{
    // org.bluez exports ObjectManager, but there is no AgentManager1
    // expected: init error
    FakeBluez::start();
    FakeBluez::runTest(QStringLiteral("bluez-empty-managed-objects"));

    Manager *manager = new Manager;
    InitManagerJob *job = manager->init();
    job->exec();

    QVERIFY(job->error());
    QVERIFY(!manager->isInitialized());
    QVERIFY(!manager->isOperational());
    QVERIFY(!manager->isBluetoothOperational());
}

void ManagerTest::bluezNoAdaptersTest()
{
    // org.bluez exports ObjectManager with AgentManager1, but there are no adapters
    // expected: init successful
    FakeBluez::start();
    FakeBluez::runTest(QStringLiteral("bluez-no-adapters"));

    Manager *manager = new Manager;
    InitManagerJob *job = manager->init();
    job->exec();

    QVERIFY(!job->error());
    QVERIFY(manager->isInitialized());
    QVERIFY(manager->isOperational());
    QVERIFY(!manager->isBluetoothOperational());
}

QTEST_MAIN(ManagerTest)
