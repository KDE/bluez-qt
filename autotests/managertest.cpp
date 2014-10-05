#include "managertest.h"
#include "autotests.h"
#include "manager.h"
#include "initmanagerjob.h"
#include "adapter.h"
#include "device.h"

#include <QtTest/QTest>
#include <QtTest/QSignalSpy>
#include <QLoggingCategory>
#include <QDBusObjectPath>

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

void ManagerTest::bluezRestartTest()
{
    // tests whether the adapter/device removed signals work correctly

    FakeBluez::start();
    FakeBluez::runTest(QStringLiteral("bluez-standard"));

    // Create adapters
    QDBusObjectPath adapter1path = QDBusObjectPath(QStringLiteral("/org/bluez/hci0"));
    QVariantMap adapterProps;
    adapterProps[QStringLiteral("Path")] = QVariant::fromValue(adapter1path);
    adapterProps[QStringLiteral("Address")] = QStringLiteral("1C:E5:C3:BC:94:7E");
    adapterProps[QStringLiteral("Name")] = QStringLiteral("TestAdapter");
    adapterProps[QStringLiteral("Powered")] = true;
    FakeBluez::runAction(QStringLiteral("devicemanager"), QStringLiteral("create-adapter"), adapterProps);

    QDBusObjectPath adapter2path = QDBusObjectPath(QStringLiteral("/org/bluez/hci1"));
    adapterProps[QStringLiteral("Path")] = QVariant::fromValue(adapter2path);
    adapterProps[QStringLiteral("Address")] = QStringLiteral("2E:3A:C3:BC:85:7C");
    adapterProps[QStringLiteral("Name")] = QStringLiteral("TestAdapter2");
    FakeBluez::runAction(QStringLiteral("devicemanager"), QStringLiteral("create-adapter"), adapterProps);

    // Create devices
    QVariantMap deviceProps;
    deviceProps[QStringLiteral("Path")] = QVariant::fromValue(QDBusObjectPath("/org/bluez/hci0/dev_40_79_6A_0C_39_75"));
    deviceProps[QStringLiteral("Adapter")] = QVariant::fromValue(adapter1path);
    deviceProps[QStringLiteral("Address")] = QStringLiteral("40:79:6A:0C:39:75");
    deviceProps[QStringLiteral("Name")] = QStringLiteral("TestDevice");
    FakeBluez::runAction(QStringLiteral("devicemanager"), QStringLiteral("create-device"), deviceProps);

    deviceProps[QStringLiteral("Path")] = QVariant::fromValue(QDBusObjectPath("/org/bluez/hci1/dev_50_79_6A_0C_39_75"));
    deviceProps[QStringLiteral("Adapter")] = QVariant::fromValue(adapter2path);
    deviceProps[QStringLiteral("Address")] = QStringLiteral("50:79:6A:0C:39:75");
    deviceProps[QStringLiteral("Name")] = QStringLiteral("TestDevice2");
    FakeBluez::runAction(QStringLiteral("devicemanager"), QStringLiteral("create-device"), deviceProps);

    Manager *manager = new Manager;

    QSignalSpy btOperationalChangedSpy(manager, SIGNAL(bluetoothOperationalChanged(bool)));

    InitManagerJob *job = manager->init();
    job->exec();

    QVERIFY(!job->error());
    QVERIFY(manager->isInitialized());
    QVERIFY(manager->isOperational());
    QVERIFY(manager->isBluetoothOperational());
    QCOMPARE(manager->adapters().count(), 2);
    QCOMPARE(manager->devices().count(), 2);

    QCOMPARE(btOperationalChangedSpy.count(), 1);
    QCOMPARE(btOperationalChangedSpy.first().first().toBool(), true);

    Adapter *adapter1 = manager->adapterForAddress(QStringLiteral("1C:E5:C3:BC:94:7E"));
    Adapter *adapter2 = manager->adapterForAddress(QStringLiteral("2E:3A:C3:BC:85:7C"));
    Device *device1 = manager->deviceForAddress(QStringLiteral("40:79:6A:0C:39:75"));
    Device *device2 = manager->deviceForAddress(QStringLiteral("50:79:6A:0C:39:75"));

    QVERIFY(adapter1);
    QVERIFY(adapter2);
    QVERIFY(device1);
    QVERIFY(device2);

    QSignalSpy allAdaptersRemovedSpy(manager, SIGNAL(allAdaptersRemoved()));
    QSignalSpy adapterRemovedSpy(manager, SIGNAL(adapterRemoved(QBluez::Adapter*)));
    QSignalSpy device1RemovedSpy(adapter1, SIGNAL(deviceRemoved(QBluez::Device*)));
    QSignalSpy device2RemovedSpy(adapter2, SIGNAL(deviceRemoved(QBluez::Device*)));

    btOperationalChangedSpy.clear();
    FakeBluez::stop();

    // allAdaptersRemoved will be last signal
    allAdaptersRemovedSpy.wait();

    QCOMPARE(allAdaptersRemovedSpy.count(), 1);
    QCOMPARE(adapterRemovedSpy.count(), 2);
    QCOMPARE(device1RemovedSpy.count(), 1);
    QCOMPARE(device2RemovedSpy.count(), 1);

    QCOMPARE(btOperationalChangedSpy.count(), 1);
    QCOMPARE(btOperationalChangedSpy.first().first().toBool(), false);
}

QTEST_MAIN(ManagerTest)
