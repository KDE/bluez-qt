#include "devicetest.h"
#include "autotests.h"
#include "pendingcall.h"
#include "initmanagerjob.h"

#include <QtTest/QTest>
#include <QtTest/QSignalSpy>
#include <QDebug>

namespace BluezQt
{
extern void bluezqt_initFakeBluezTestRun();
}

using namespace BluezQt;

DeviceTest::DeviceTest(bool fakeBluezRun)
    : m_manager(0)
    , m_fakeBluezRun(fakeBluezRun)
{
    Autotests::registerMetatypes();
}

void DeviceTest::initTestCase()
{
    QString service = QStringLiteral("org.bluez");
    QDBusConnection connection = QDBusConnection::systemBus();

    if (!m_fakeBluezRun) {
        if (!Autotests::isBluez5Running()) {
            QSKIP("This test can only run with functional Bluez 5 org.bluez service");
        }
    } else {
        service = QStringLiteral("org.kde.bluezqt.fakebluez");
        connection = QDBusConnection::sessionBus();

        bluezqt_initFakeBluezTestRun();
        FakeBluez::start();
        FakeBluez::runTest(QStringLiteral("bluez-standard"));

        // Create adapters
        QDBusObjectPath adapter1 = QDBusObjectPath(QStringLiteral("/org/bluez/hci0"));
        QVariantMap adapterProps;
        adapterProps[QStringLiteral("Path")] = QVariant::fromValue(adapter1);
        adapterProps[QStringLiteral("Address")] = QStringLiteral("1C:E5:C3:BC:94:7E");
        adapterProps[QStringLiteral("Name")] = QStringLiteral("TestAdapter");
        FakeBluez::runAction(QStringLiteral("devicemanager"), QStringLiteral("create-adapter"), adapterProps);

        QDBusObjectPath adapter2 = QDBusObjectPath(QStringLiteral("/org/bluez/hci1"));
        adapterProps[QStringLiteral("Path")] = QVariant::fromValue(adapter2);
        adapterProps[QStringLiteral("Address")] = QStringLiteral("2E:3A:C3:BC:85:7C");
        adapterProps[QStringLiteral("Name")] = QStringLiteral("TestAdapter2");
        FakeBluez::runAction(QStringLiteral("devicemanager"), QStringLiteral("create-adapter"), adapterProps);

        // Create devices
        QVariantMap deviceProps;
        deviceProps[QStringLiteral("Path")] = QVariant::fromValue(QDBusObjectPath("/org/bluez/hci0/dev_40_79_6A_0C_39_75"));
        deviceProps[QStringLiteral("Adapter")] = QVariant::fromValue(QDBusObjectPath("/org/bluez/hci0"));
        deviceProps[QStringLiteral("Address")] = QStringLiteral("40:79:6A:0C:39:75");
        deviceProps[QStringLiteral("Name")] = QStringLiteral("TestDevice");
        deviceProps[QStringLiteral("Alias")] = QStringLiteral("TestAlias");
        deviceProps[QStringLiteral("Icon")] = QStringLiteral("phone");
        deviceProps[QStringLiteral("Class")] = QVariant::fromValue(quint32(101));
        deviceProps[QStringLiteral("Appearance")] = QVariant::fromValue(quint16(25));
        deviceProps[QStringLiteral("UUIDs")] = QStringList();
        deviceProps[QStringLiteral("Paired")] = false;
        deviceProps[QStringLiteral("Connected")] = false;
        deviceProps[QStringLiteral("Trusted")] = false;
        deviceProps[QStringLiteral("Blocked")] = false;
        deviceProps[QStringLiteral("LegacyPairing")] = false;
        deviceProps[QStringLiteral("RSSI")] = QVariant::fromValue(qint16(20));
        deviceProps[QStringLiteral("Modalias")] = QStringLiteral("bluetooth:v001Dp1200d1236");
        FakeBluez::runAction(QStringLiteral("devicemanager"), QStringLiteral("create-device"), deviceProps);

        deviceProps[QStringLiteral("Path")] = QVariant::fromValue(QDBusObjectPath("/org/bluez/hci1/dev_50_79_6A_0C_39_75"));
        deviceProps[QStringLiteral("Adapter")] = QVariant::fromValue(QDBusObjectPath("/org/bluez/hci1"));
        deviceProps[QStringLiteral("Address")] = QStringLiteral("50:79:6A:0C:39:75");
        deviceProps[QStringLiteral("Name")] = QStringLiteral("TestDevice2");
        deviceProps[QStringLiteral("Alias")] = QStringLiteral("TestAlias2");
        deviceProps[QStringLiteral("Icon")] = QStringLiteral("joypad");
        deviceProps[QStringLiteral("Class")] = QVariant::fromValue(quint32(201));
        deviceProps[QStringLiteral("Appearance")] = QVariant::fromValue(quint16(32));
        deviceProps[QStringLiteral("UUIDs")] = QStringList();
        deviceProps[QStringLiteral("Paired")] = true;
        deviceProps[QStringLiteral("Connected")] = false;
        deviceProps[QStringLiteral("Trusted")] = true;
        deviceProps[QStringLiteral("Blocked")] = false;
        deviceProps[QStringLiteral("LegacyPairing")] = false;
        deviceProps[QStringLiteral("RSSI")] = QVariant::fromValue(qint16(-15));
        deviceProps[QStringLiteral("Modalias")] = QStringLiteral("bluetooth:v001Dp1100d1236");
        FakeBluez::runAction(QStringLiteral("devicemanager"), QStringLiteral("create-device"), deviceProps);
    }

    m_manager = new Manager();
    InitManagerJob *initJob = m_manager->init();
    initJob->exec();
    QVERIFY(!initJob->error());

    Q_FOREACH (AdapterPtr adapter, m_manager->adapters()) {
        QVERIFY(!adapter->ubi().isEmpty());

        Q_FOREACH (DevicePtr device, adapter->devices()) {
            QVERIFY(!device->ubi().isEmpty());

            DeviceUnit u;
            u.device = device;
            u.dbusDevice = new org::bluez::Device1(service,
                                                   device->ubi(),
                                                   connection,
                                                   this);
            u.dbusProperties = new org::freedesktop::DBus::Properties(service,
                                                                      device->ubi(),
                                                                      connection,
                                                                      this);
            m_units.append(u);
        }
    }

    if (m_fakeBluezRun) {
        QCOMPARE(m_manager->adapters().count(), 2);
        QCOMPARE(m_manager->devices().count(), 2);
    } else {
        qDebug() << "Got" << m_units.count() << "devices.";
    }
}

void DeviceTest::cleanupTestCase()
{
    Q_FOREACH (const DeviceUnit &unit, m_units) {
        delete unit.dbusDevice;
        delete unit.dbusProperties;
    }

    delete m_manager;

    if (m_fakeBluezRun) {
        FakeBluez::stop();
    }
}

static void compareUuids(const QStringList &actual, const QStringList &expected)
{
    QCOMPARE(actual.size(), expected.size());

    for (int i = 0; i < actual.size(); ++i) {
        QCOMPARE(actual.at(i).toUpper(), expected.at(i).toUpper());
    }
}

void DeviceTest::getPropertiesTest()
{
    Q_FOREACH (const DeviceUnit &unit, m_units) {
        QCOMPARE(unit.device->ubi(), unit.dbusDevice->path());
        QCOMPARE(unit.device->address(), unit.dbusDevice->address());
        QCOMPARE(unit.device->name(), unit.dbusDevice->alias());
        QCOMPARE(unit.device->remoteName(), unit.dbusDevice->name());
        QCOMPARE(unit.device->deviceClass(), unit.dbusDevice->deviceClass());
        QCOMPARE(unit.device->appearance(), unit.dbusDevice->appearance());
        QCOMPARE(unit.device->icon(), unit.dbusDevice->icon());
        QCOMPARE(unit.device->isPaired(), unit.dbusDevice->paired());
        QCOMPARE(unit.device->isTrusted(), unit.dbusDevice->trusted());
        QCOMPARE(unit.device->isBlocked(), unit.dbusDevice->blocked());
        QCOMPARE(unit.device->hasLegacyPairing(), unit.dbusDevice->legacyPairing());
        QCOMPARE(unit.device->rssi(), unit.dbusDevice->rSSI());
        QCOMPARE(unit.device->isConnected(), unit.dbusDevice->connected());
        QCOMPARE(unit.device->modalias(), unit.dbusDevice->modalias());
        QCOMPARE(unit.device->adapter()->ubi(), unit.dbusDevice->adapter().path());

        compareUuids(unit.device->uuids(), unit.dbusDevice->uUIDs());
    }
}

void DeviceTest::setAliasTest()
{
    Q_FOREACH (const DeviceUnit &unit, m_units) {
        QSignalSpy deviceSpy(unit.device.data(), SIGNAL(nameChanged(QString)));
        QSignalSpy dbusSpy(unit.dbusProperties, SIGNAL(PropertiesChanged(QString,QVariantMap,QStringList)));

        QString originalValue = unit.device->name();
        QString value = originalValue + QLatin1String("_tst_alias");

        unit.device->setName(value);
        QTRY_COMPARE(deviceSpy.count(), 1);

        QList<QVariant> arguments = deviceSpy.takeFirst();
        QCOMPARE(arguments.at(0).toString(), value);
        Autotests::verifyPropertiesChangedSignal(dbusSpy, QStringLiteral("Alias"), value);

        QCOMPARE(unit.device->name(), value);
        QCOMPARE(unit.dbusDevice->alias(), value);

        unit.device->setName(originalValue)->waitForFinished();
    }
}

void DeviceTest::setTrustedTest()
{
    Q_FOREACH (const DeviceUnit &unit, m_units) {
        QSignalSpy deviceSpy(unit.device.data(), SIGNAL(trustedChanged(bool)));
        QSignalSpy dbusSpy(unit.dbusProperties, SIGNAL(PropertiesChanged(QString,QVariantMap,QStringList)));

        bool originalValue = unit.device->isTrusted();
        bool value = !originalValue;

        unit.device->setTrusted(value);
        QTRY_COMPARE(deviceSpy.count(), 1);

        QList<QVariant> arguments = deviceSpy.takeFirst();
        QCOMPARE(arguments.at(0).toBool(), value);
        Autotests::verifyPropertiesChangedSignal(dbusSpy, QStringLiteral("Trusted"), value);

        QCOMPARE(unit.device->isTrusted(), value);
        QCOMPARE(unit.dbusDevice->trusted(), value);

        unit.device->setTrusted(originalValue)->waitForFinished();
    }
}

void DeviceTest::setBlockedTest()
{
    Q_FOREACH (const DeviceUnit &unit, m_units) {
        QSignalSpy deviceSpy(unit.device.data(), SIGNAL(blockedChanged(bool)));
        QSignalSpy dbusSpy(unit.dbusProperties, SIGNAL(PropertiesChanged(QString,QVariantMap,QStringList)));

        bool originalValue = unit.device->isBlocked();
        bool value = !originalValue;

        unit.device->setBlocked(value);
        QTRY_COMPARE(deviceSpy.count(), 1);

        QList<QVariant> arguments = deviceSpy.takeFirst();
        QCOMPARE(arguments.at(0).toBool(), value);
        Autotests::verifyPropertiesChangedSignal(dbusSpy, QStringLiteral("Blocked"), value);

        QCOMPARE(unit.device->isBlocked(), value);
        QCOMPARE(unit.dbusDevice->blocked(), value);

        unit.device->setBlocked(originalValue)->waitForFinished();
    }
}

void DeviceTest::deviceRemovedTest()
{
    if (!m_fakeBluezRun) {
        return;
    }

    Q_FOREACH (const DeviceUnit &unit, m_units) {
        QSignalSpy managerSpy(m_manager, SIGNAL(deviceRemoved(DevicePtr)));
        QSignalSpy adapterSpy(unit.device->adapter().data(), SIGNAL(deviceRemoved(DevicePtr)));
        QSignalSpy deviceSpy(unit.device.data(), SIGNAL(deviceRemoved(DevicePtr)));

        QVariantMap properties;
        properties[QStringLiteral("Path")] = QVariant::fromValue(QDBusObjectPath(unit.device->ubi()));
        FakeBluez::runAction(QStringLiteral("devicemanager"), QStringLiteral("remove-device"), properties);

        QTRY_COMPARE(managerSpy.count(), 1);
        QTRY_COMPARE(adapterSpy.count(), 1);
        QTRY_COMPARE(deviceSpy.count(), 1);

        QCOMPARE(managerSpy.at(0).at(0).value<DevicePtr>(), unit.device);
        QCOMPARE(adapterSpy.at(0).at(0).value<DevicePtr>(), unit.device);
        QCOMPARE(deviceSpy.at(0).at(0).value<DevicePtr>(), unit.device);
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    DeviceTest test1(false /*fakeBluezRun*/);
    int res = QTest::qExec(&test1, argc, argv);
    if (res != 0) {
        return res;
    }

    DeviceTest test2(true /*fakeBluezRun*/);
    return QTest::qExec(&test2, argc, argv);
}
