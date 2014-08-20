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

QTEST_MAIN(ManagerTest)
