#include "devicetest.h"
#include "autotests.h"
#include "pendingcall.h"
#include "initmanagerjob.h"
#include "loaddevicejob.h"

#include <QtTest/QTest>
#include <QtTest/QSignalSpy>
#include <QDebug>

using namespace QBluez;

void DeviceTest::initTestCase()
{
    Manager *manager = new Manager();
    InitManagerJob *initJob = manager->init(Manager::InitManagerAndAdapters);
    initJob->exec();
    QVERIFY(!initJob->error());

    Q_FOREACH (Adapter *adapter, manager->adapters()) {
        QVERIFY(!adapter->ubi().isEmpty());

        Q_FOREACH (Device *device, adapter->devices()) {
            QVERIFY(!device->ubi().isEmpty());

            LoadDeviceJob *deviceJob = device->load();
            deviceJob->exec();
            QVERIFY(deviceJob->error() == LoadDeviceJob::NoError);
            QVERIFY(device->isLoaded());

            DeviceUnit u;
            u.device = device;
            u.dbusDevice = new org::bluez::Device1(QStringLiteral("org.bluez"),
                                                   device->ubi(),
                                                   QDBusConnection::systemBus(),
                                                   this);
            u.dbusProperties = new org::freedesktop::DBus::Properties(QStringLiteral("org.bluez"),
                                                                      device->ubi(),
                                                                      QDBusConnection::systemBus(),
                                                                      this);
            m_units.append(u);
        }
    }

    qDebug() << "Got" << m_units.count() << "devices.";
}

void DeviceTest::cleanupTestCase()
{
    Q_FOREACH (const DeviceUnit &unit, m_units) {
        delete unit.dbusDevice;
        delete unit.dbusProperties;
    }
}

void DeviceTest::getPropertiesTest()
{
    Q_FOREACH (const DeviceUnit &unit, m_units) {
        QCOMPARE(unit.device->address(), unit.dbusDevice->address());
        QCOMPARE(unit.device->name(), unit.dbusDevice->name());
        QCOMPARE(unit.device->alias(), unit.dbusDevice->alias());
        QCOMPARE(unit.device->deviceClass(), unit.dbusDevice->deviceClass());
        QCOMPARE(unit.device->appearance(), unit.dbusDevice->appearance());
        QCOMPARE(unit.device->icon(), unit.dbusDevice->icon());
        QCOMPARE(unit.device->isPaired(), unit.dbusDevice->paired());
        QCOMPARE(unit.device->isTrusted(), unit.dbusDevice->trusted());
        QCOMPARE(unit.device->isBlocked(), unit.dbusDevice->blocked());
        QCOMPARE(unit.device->hasLegacyPairing(), unit.dbusDevice->legacyPairing());
        QCOMPARE(unit.device->rssi(), unit.dbusDevice->rSSI());
        QCOMPARE(unit.device->isConnected(), unit.dbusDevice->connected());
        QCOMPARE(unit.device->uuids(), unit.dbusDevice->uUIDs());
        QCOMPARE(unit.device->modalias(), unit.dbusDevice->modalias());
        QCOMPARE(unit.device->adapter()->ubi(), unit.dbusDevice->adapter().path());
    }
}

void DeviceTest::setAliasTest()
{
    Q_FOREACH (const DeviceUnit &unit, m_units) {
        QSignalSpy deviceSpy(unit.device, SIGNAL(aliasChanged(QString)));
        QSignalSpy dbusSpy(unit.dbusProperties, SIGNAL(PropertiesChanged(QString,QVariantMap,QStringList)));

        QString originalValue = unit.device->alias();
        QString value = originalValue + QLatin1String("_tst_alias");

        unit.device->setAlias(value);
        deviceSpy.wait();
        QCOMPARE(deviceSpy.count(), 1);

        QList<QVariant> arguments = deviceSpy.takeFirst();
        QCOMPARE(arguments.at(0).toString(), value);
        verifyPropertiesChangedSignal(dbusSpy, QStringLiteral("Alias"), value);

        QCOMPARE(unit.device->alias(), value);
        QCOMPARE(unit.dbusDevice->alias(), value);

        unit.device->setAlias(originalValue)->waitForFinished();
    }
}

void DeviceTest::setTrustedTest()
{
    Q_FOREACH (const DeviceUnit &unit, m_units) {
        QSignalSpy deviceSpy(unit.device, SIGNAL(trustedChanged(bool)));
        QSignalSpy dbusSpy(unit.dbusProperties, SIGNAL(PropertiesChanged(QString,QVariantMap,QStringList)));

        bool originalValue = unit.device->isTrusted();
        bool value = !originalValue;

        unit.device->setTrusted(value);
        deviceSpy.wait();
        QCOMPARE(deviceSpy.count(), 1);

        QList<QVariant> arguments = deviceSpy.takeFirst();
        QCOMPARE(arguments.at(0).toBool(), value);
        verifyPropertiesChangedSignal(dbusSpy, QStringLiteral("Trusted"), value);

        QCOMPARE(unit.device->isTrusted(), value);
        QCOMPARE(unit.dbusDevice->trusted(), value);

        unit.device->setTrusted(originalValue)->waitForFinished();
    }
}

void DeviceTest::setBlockedTest()
{
    Q_FOREACH (const DeviceUnit &unit, m_units) {
        QSignalSpy deviceSpy(unit.device, SIGNAL(blockedChanged(bool)));
        QSignalSpy dbusSpy(unit.dbusProperties, SIGNAL(PropertiesChanged(QString,QVariantMap,QStringList)));

        bool originalValue = unit.device->isBlocked();
        bool value = !originalValue;

        unit.device->setBlocked(value);
        deviceSpy.wait();
        QCOMPARE(deviceSpy.count(), 1);

        QList<QVariant> arguments = deviceSpy.takeFirst();
        QCOMPARE(arguments.at(0).toBool(), value);
        verifyPropertiesChangedSignal(dbusSpy, QStringLiteral("Blocked"), value);

        QCOMPARE(unit.device->isBlocked(), value);
        QCOMPARE(unit.dbusDevice->blocked(), value);

        unit.device->setBlocked(originalValue)->waitForFinished();
    }
}

QTEST_MAIN(DeviceTest)
