#include "adaptertest.h"
#include "autotests.h"

#include <QtTest/QTest>
#include <QtTest/QSignalSpy>
#include <QDebug>

#include "loadadaptersjob.h"

using namespace QBluez;

void AdapterTest::initTestCase()
{
    GetManagerJob *managerJob = Manager::get();
    managerJob->exec();
    QVERIFY(managerJob->error() == GetManagerJob::NoError);

    Manager *manager = managerJob->manager();
    QVERIFY(manager);

    LoadAdaptersJob *adaptersJob = manager->loadAdapters();
    adaptersJob->exec();
    QVERIFY(adaptersJob->error() == LoadAdaptersJob::NoError);

    Q_FOREACH (Adapter *adapter, manager->adapters()) {
        QVERIFY(!adapter->path().isEmpty());

        AdapterUnit u;
        u.adapter = adapter;
        u.dbusAdapter = new org::bluez::Adapter1(QStringLiteral("org.bluez"),
                                                 adapter->path(),
                                                 QDBusConnection::systemBus(),
                                                 this);
        u.dbusProperties = new org::freedesktop::DBus::Properties(QStringLiteral("org.bluez"),
                                                                  adapter->path(),
                                                                  QDBusConnection::systemBus(),
                                                                  this);
        m_units.append(u);
    }

    qDebug() << "Got" << m_units.count() << "adapters.";
}

void AdapterTest::cleanupTestCase()
{
    Q_FOREACH (const AdapterUnit &unit, m_units) {
        delete unit.dbusAdapter;
        delete unit.dbusProperties;
    }
}

void AdapterTest::getPropertiesTest()
{
    Q_FOREACH (const AdapterUnit &unit, m_units) {
        QCOMPARE(unit.adapter->address(), unit.dbusAdapter->address());
        QCOMPARE(unit.adapter->name(), unit.dbusAdapter->name());
        QCOMPARE(unit.adapter->alias(), unit.dbusAdapter->alias());
        QCOMPARE(unit.adapter->adapterClass(), unit.dbusAdapter->adapterClass());
        QCOMPARE(unit.adapter->isPowered(), unit.dbusAdapter->powered());
        QCOMPARE(unit.adapter->isDiscoverable(), unit.dbusAdapter->discoverable());
        QCOMPARE(unit.adapter->discoverableTimeout(), unit.dbusAdapter->discoverableTimeout());
        QCOMPARE(unit.adapter->isPairable(), unit.dbusAdapter->pairable());
        QCOMPARE(unit.adapter->pairableTimeout(), unit.dbusAdapter->pairableTimeout());
        QCOMPARE(unit.adapter->isDiscovering(), unit.dbusAdapter->discovering());
        QCOMPARE(unit.adapter->uuids(), unit.dbusAdapter->uUIDs());
        QCOMPARE(unit.adapter->modalias(), unit.dbusAdapter->modalias());
    }
}

void AdapterTest::setAliasTest()
{
    Q_FOREACH (const AdapterUnit &unit, m_units) {
        QSignalSpy adapterSpy(unit.adapter, SIGNAL(aliasChanged(QString)));
        QSignalSpy dbusSpy(unit.dbusProperties, SIGNAL(PropertiesChanged(QString,QVariantMap,QStringList)));

        QString originalValue = unit.adapter->alias();
        QString value = originalValue + QLatin1String("_tst_alias");

        unit.adapter->setAlias(value)->start();
        adapterSpy.wait();
        QCOMPARE(adapterSpy.count(), 1);

        QList<QVariant> adapterArguments = adapterSpy.takeFirst();
        QCOMPARE(adapterArguments.at(0).toString(), value);
        verifyPropertiesChangedSignal(dbusSpy, QStringLiteral("Alias"), value);

        QCOMPARE(unit.adapter->alias(), value);
        QCOMPARE(unit.dbusAdapter->alias(), value);

        unit.adapter->setAlias(originalValue)->exec();
    }
}

void AdapterTest::setPoweredTest()
{
    Q_FOREACH (const AdapterUnit &unit, m_units) {
        QSignalSpy adapterSpy(unit.adapter, SIGNAL(poweredChanged(bool)));
        QSignalSpy dbusSpy(unit.dbusProperties, SIGNAL(PropertiesChanged(QString,QVariantMap,QStringList)));

        bool originalValue = unit.adapter->isPowered();
        bool value = !originalValue;

        unit.adapter->setPowered(value)->start();
        adapterSpy.wait();
        QCOMPARE(adapterSpy.count(), 1);

        QVERIFY(dbusSpy.count() >= 1);
        verifyPropertiesChangedSignal(dbusSpy, QStringLiteral("Powered"), value);

        QList<QVariant> adapterArguments = adapterSpy.takeFirst();
        QCOMPARE(adapterArguments.at(0).toBool(), value);

        QCOMPARE(unit.adapter->isPowered(), value);
        QCOMPARE(unit.dbusAdapter->powered(), value);

        unit.adapter->setPowered(originalValue)->exec();
    }
}

void AdapterTest::setDiscoverableTest()
{
    // Discoverable cannot be changed when Adapter is off

    Q_FOREACH (const AdapterUnit &unit, m_units) {
        bool wasPowered = unit.adapter->isPowered();
        unit.adapter->setPowered(true)->exec();

        QSignalSpy adapterSpy(unit.adapter, SIGNAL(discoverableChanged(bool)));
        QSignalSpy dbusSpy(unit.dbusProperties, SIGNAL(PropertiesChanged(QString,QVariantMap,QStringList)));

        bool originalValue = unit.adapter->isDiscoverable();
        bool value = !originalValue;

        unit.adapter->setDiscoverable(value)->start();
        adapterSpy.wait();
        QCOMPARE(adapterSpy.count(), 1);

        QList<QVariant> adapterArguments = adapterSpy.takeFirst();
        QCOMPARE(adapterArguments.at(0).toBool(), value);
        verifyPropertiesChangedSignal(dbusSpy, QStringLiteral("Discoverable"), value);

        QCOMPARE(unit.adapter->isDiscoverable(), value);
        QCOMPARE(unit.dbusAdapter->discoverable(), value);

        unit.adapter->setDiscoverable(originalValue)->exec();
        unit.adapter->setPowered(wasPowered)->exec();
    }
}

void AdapterTest::setDiscoverableTimeoutTest()
{
    Q_FOREACH (const AdapterUnit &unit, m_units) {
        QSignalSpy adapterSpy(unit.adapter, SIGNAL(discoverableTimeoutChanged(quint32)));
        QSignalSpy dbusSpy(unit.dbusProperties, SIGNAL(PropertiesChanged(QString,QVariantMap,QStringList)));

        quint32 originalValue = unit.adapter->discoverableTimeout();
        quint32 value = originalValue + 1;

        unit.adapter->setDiscoverableTimeout(value)->start();
        adapterSpy.wait();
        QCOMPARE(adapterSpy.count(), 1);

        QVERIFY(dbusSpy.count() >= 1);
        verifyPropertiesChangedSignal(dbusSpy, QStringLiteral("DiscoverableTimeout"), value);

        QList<QVariant> adapterArguments = adapterSpy.takeFirst();
        QCOMPARE(adapterArguments.at(0).toUInt(), value);

        QCOMPARE(unit.adapter->discoverableTimeout(), value);
        QCOMPARE(unit.dbusAdapter->discoverableTimeout(), value);

        unit.adapter->setDiscoverableTimeout(originalValue)->exec();
    }
}

void AdapterTest::setPairableTest()
{
    Q_FOREACH (const AdapterUnit &unit, m_units) {
        QSignalSpy adapterSpy(unit.adapter, SIGNAL(pairableChanged(bool)));
        QSignalSpy dbusSpy(unit.dbusProperties, SIGNAL(PropertiesChanged(QString,QVariantMap,QStringList)));

        bool originalValue = unit.adapter->isPairable();
        bool value = !originalValue;

        unit.adapter->setPairable(value)->start();
        adapterSpy.wait();
        QCOMPARE(adapterSpy.count(), 1);

        QList<QVariant> adapterArguments = adapterSpy.takeFirst();
        QCOMPARE(adapterArguments.at(0).toBool(), value);
        verifyPropertiesChangedSignal(dbusSpy, QStringLiteral("Pairable"), value);

        QCOMPARE(unit.adapter->isPairable(), value);
        QCOMPARE(unit.dbusAdapter->pairable(), value);

        unit.adapter->setPairable(originalValue)->exec();
    }
}

void AdapterTest::setPairableTimeoutTest()
{
    QSKIP("Does not pass on my system because multiple properties changed signals are emitted...", QTest::SkipSingle);

    Q_FOREACH (const AdapterUnit &unit, m_units) {
        QSignalSpy adapterSpy(unit.adapter, SIGNAL(pairableTimeoutChanged(quint32)));
        QSignalSpy dbusSpy(unit.dbusProperties, SIGNAL(PropertiesChanged(QString,QVariantMap,QStringList)));

        quint32 originalValue = unit.adapter->pairableTimeout();
        quint32 value = originalValue + 1;

        unit.adapter->setPairableTimeout(value)->start();
        adapterSpy.wait();
        QCOMPARE(adapterSpy.count(), 1);

        QVERIFY(dbusSpy.count() >= 1);
        verifyPropertiesChangedSignal(dbusSpy, QStringLiteral("PairableTimeout"), value);

        QList<QVariant> adapterArguments = adapterSpy.takeFirst();
        QCOMPARE(adapterArguments.at(0).toUInt(), value);

        QCOMPARE(unit.adapter->pairableTimeout(), value);
        QCOMPARE(unit.dbusAdapter->pairableTimeout(), value);

        unit.adapter->setPairableTimeout(originalValue)->exec();
    }
}

void AdapterTest::discoveryTest()
{
    // Discovery needs Adapter powered on

    Q_FOREACH (const AdapterUnit &unit, m_units) {
        // Make sure the Adapter is powered on and not discovering
        bool wasPowered = unit.adapter->isPowered();
        unit.adapter->setPowered(true)->exec();
        unit.dbusAdapter->StopDiscovery();
        QTest::qWait(50);

        QSignalSpy adapterSpy(unit.adapter, SIGNAL(discoveringChanged(bool)));
        QSignalSpy dbusSpy(unit.dbusProperties, SIGNAL(PropertiesChanged(QString,QVariantMap,QStringList)));

        // Start Discovery
        unit.adapter->startDiscovery();
        adapterSpy.wait();
        QCOMPARE(adapterSpy.count(), 1);

        QVERIFY(dbusSpy.count() >= 1);
        verifyPropertiesChangedSignal(dbusSpy, QStringLiteral("Discovering"), true);

        QList<QVariant> adapterArguments = adapterSpy.takeFirst();
        QCOMPARE(adapterArguments.at(0).toBool(), true);

        QCOMPARE(unit.adapter->isDiscovering(), true);
        QCOMPARE(unit.dbusAdapter->discovering(), true);

        adapterSpy.clear();
        dbusSpy.clear();

        // Stop Discovery
        unit.adapter->stopDiscovery();
        adapterSpy.wait();
        QCOMPARE(adapterSpy.count(), 1);

        QVERIFY(dbusSpy.count() >= 1);
        verifyPropertiesChangedSignal(dbusSpy, QStringLiteral("Discovering"), false);

        adapterArguments = adapterSpy.takeFirst();
        QCOMPARE(adapterArguments.at(0).toBool(), false);

        QCOMPARE(unit.adapter->isDiscovering(), false);
        QCOMPARE(unit.dbusAdapter->discovering(), false);

        unit.adapter->setPowered(wasPowered)->exec();
    }
}

QTEST_MAIN(AdapterTest)

