#include "adaptertest.h"
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

AdapterTest::AdapterTest(bool fakeBluezRun)
    : m_manager(0)
    , m_fakeBluezRun(fakeBluezRun)
{
}

void AdapterTest::initTestCase()
{
    QString service = QStringLiteral("org.bluez");
    QDBusConnection connection = QDBusConnection::systemBus();

    if (!m_fakeBluezRun) {
        if (!isBluez5Running()) {
            QSKIP("This test can only run with functional Bluez 5 org.bluez service");
        }
    } else {
        service = QStringLiteral("org.bluezqt.fakebluez");
        connection = QDBusConnection::sessionBus();

        bluezqt_initFakeBluezTestRun();
        FakeBluez::start();
        FakeBluez::runTest(QStringLiteral("bluez-standard"));

        // Create adapters
        QVariantMap adapterProps;
        adapterProps[QStringLiteral("Path")] = QVariant::fromValue(QDBusObjectPath(QStringLiteral("/org/bluez/hci0")));
        adapterProps[QStringLiteral("Address")] = QStringLiteral("1C:E5:C3:BC:94:7E");
        adapterProps[QStringLiteral("Name")] = QStringLiteral("TestAdapter");
        adapterProps[QStringLiteral("Alias")] = QStringLiteral("TestAlias");
        adapterProps[QStringLiteral("Class")] = QVariant::fromValue(quint32(101));
        adapterProps[QStringLiteral("Powered")] = false;
        adapterProps[QStringLiteral("Discoverable")] = false;
        adapterProps[QStringLiteral("Pairable")] = false;
        adapterProps[QStringLiteral("PairableTimeout")] = QVariant::fromValue(quint32(0));
        adapterProps[QStringLiteral("DiscoverableTimeout")] = QVariant::fromValue(quint32(0));
        adapterProps[QStringLiteral("Discovering")] = false;
        adapterProps[QStringLiteral("UUIDs")] = QStringList(QStringLiteral("00001200-0000-1000-8000-00805f9b34fb"));
        adapterProps[QStringLiteral("Modalias")] = QStringLiteral("usb:v2D6Bp1236d0215");
        FakeBluez::runAction(QStringLiteral("devicemanager"), QStringLiteral("create-adapter"), adapterProps);

        adapterProps[QStringLiteral("Path")] = QVariant::fromValue(QDBusObjectPath(QStringLiteral("/org/bluez/hci1")));
        adapterProps[QStringLiteral("Address")] = QStringLiteral("2E:3A:C3:BC:85:7C");
        adapterProps[QStringLiteral("Name")] = QStringLiteral("TestAdapter2");
        adapterProps[QStringLiteral("Alias")] = QStringLiteral("TestAlias2");
        adapterProps[QStringLiteral("Class")] = QVariant::fromValue(quint32(201));
        adapterProps[QStringLiteral("Powered")] = true;
        adapterProps[QStringLiteral("Discoverable")] = true;
        adapterProps[QStringLiteral("Pairable")] = true;
        adapterProps[QStringLiteral("PairableTimeout")] = QVariant::fromValue(quint32(150));
        adapterProps[QStringLiteral("DiscoverableTimeout")] = QVariant::fromValue(quint32(120));
        adapterProps[QStringLiteral("Discovering")] = false;
        adapterProps[QStringLiteral("UUIDs")] = QStringList(QStringLiteral("0000110c-0000-1000-8000-00805f9b34fb"));
        adapterProps[QStringLiteral("Modalias")] = QStringLiteral("usb:v1D3Bp1134d0214");
        FakeBluez::runAction(QStringLiteral("devicemanager"), QStringLiteral("create-adapter"), adapterProps);
    }

    m_manager = new Manager();
    InitManagerJob *initJob = m_manager->init();
    initJob->exec();
    QVERIFY(!initJob->error());

    Q_FOREACH (Adapter *adapter, m_manager->adapters()) {
        QVERIFY(!adapter->ubi().isEmpty());

        AdapterUnit u;
        u.adapter = adapter;
        u.dbusAdapter = new org::bluez::Adapter1(service,
                                                 adapter->ubi(),
                                                 connection,
                                                 this);
        u.dbusProperties = new org::freedesktop::DBus::Properties(service,
                                                                  adapter->ubi(),
                                                                  connection,
                                                                  this);
        m_units.append(u);
    }

    if (m_fakeBluezRun) {
        QCOMPARE(m_manager->adapters().count(), 2);
    } else {
        qDebug() << "Got" << m_units.count() << "adapters.";
    }
}

void AdapterTest::cleanupTestCase()
{
    Q_FOREACH (const AdapterUnit &unit, m_units) {
        delete unit.dbusAdapter;
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

void AdapterTest::getPropertiesTest()
{
    Q_FOREACH (const AdapterUnit &unit, m_units) {
        QCOMPARE(unit.adapter->ubi(), unit.dbusAdapter->path());
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
        QCOMPARE(unit.adapter->modalias(), unit.dbusAdapter->modalias());

        compareUuids(unit.adapter->uuids(), unit.dbusAdapter->uUIDs());
    }
}

void AdapterTest::setAliasTest()
{
    Q_FOREACH (const AdapterUnit &unit, m_units) {
        QSignalSpy adapterSpy(unit.adapter, SIGNAL(aliasChanged(QString)));
        QSignalSpy dbusSpy(unit.dbusProperties, SIGNAL(PropertiesChanged(QString,QVariantMap,QStringList)));

        QString originalValue = unit.adapter->alias();
        QString value = originalValue + QLatin1String("_tst_alias");

        unit.adapter->setAlias(value);
        QTRY_COMPARE(adapterSpy.count(), 1);

        QList<QVariant> adapterArguments = adapterSpy.takeFirst();
        QCOMPARE(adapterArguments.at(0).toString(), value);
        verifyPropertiesChangedSignal(dbusSpy, QStringLiteral("Alias"), value);

        QCOMPARE(unit.adapter->alias(), value);
        QCOMPARE(unit.dbusAdapter->alias(), value);

        unit.adapter->setAlias(originalValue)->waitForFinished();
    }
}

void AdapterTest::setPoweredTest()
{
    Q_FOREACH (const AdapterUnit &unit, m_units) {
        QSignalSpy adapterSpy(unit.adapter, SIGNAL(poweredChanged(bool)));
        QSignalSpy dbusSpy(unit.dbusProperties, SIGNAL(PropertiesChanged(QString,QVariantMap,QStringList)));

        bool originalValue = unit.adapter->isPowered();
        bool value = !originalValue;

        unit.adapter->setPowered(value);
        QTRY_COMPARE(adapterSpy.count(), 1);

        QVERIFY(dbusSpy.count() >= 1);
        verifyPropertiesChangedSignal(dbusSpy, QStringLiteral("Powered"), value);

        QList<QVariant> adapterArguments = adapterSpy.takeFirst();
        QCOMPARE(adapterArguments.at(0).toBool(), value);

        QCOMPARE(unit.adapter->isPowered(), value);
        QCOMPARE(unit.dbusAdapter->powered(), value);

        unit.adapter->setPowered(originalValue)->waitForFinished();
    }
}

void AdapterTest::setDiscoverableTest()
{
    // Discoverable cannot be changed when Adapter is off

    Q_FOREACH (const AdapterUnit &unit, m_units) {
        bool wasPowered = unit.adapter->isPowered();
        unit.adapter->setPowered(true)->waitForFinished();

        QSignalSpy adapterSpy(unit.adapter, SIGNAL(discoverableChanged(bool)));
        QSignalSpy dbusSpy(unit.dbusProperties, SIGNAL(PropertiesChanged(QString,QVariantMap,QStringList)));

        bool originalValue = unit.adapter->isDiscoverable();
        bool value = !originalValue;

        unit.adapter->setDiscoverable(value);
        QTRY_COMPARE(adapterSpy.count(), 1);

        QList<QVariant> adapterArguments = adapterSpy.takeFirst();
        QCOMPARE(adapterArguments.at(0).toBool(), value);
        verifyPropertiesChangedSignal(dbusSpy, QStringLiteral("Discoverable"), value);

        QCOMPARE(unit.adapter->isDiscoverable(), value);
        QCOMPARE(unit.dbusAdapter->discoverable(), value);

        unit.adapter->setDiscoverable(originalValue)->waitForFinished();
        unit.adapter->setPowered(wasPowered)->waitForFinished();
    }
}

void AdapterTest::setDiscoverableTimeoutTest()
{
    Q_FOREACH (const AdapterUnit &unit, m_units) {
        QSignalSpy adapterSpy(unit.adapter, SIGNAL(discoverableTimeoutChanged(quint32)));
        QSignalSpy dbusSpy(unit.dbusProperties, SIGNAL(PropertiesChanged(QString,QVariantMap,QStringList)));

        quint32 originalValue = unit.adapter->discoverableTimeout();
        quint32 value = originalValue + 1;

        unit.adapter->setDiscoverableTimeout(value);
        QTRY_COMPARE(adapterSpy.count(), 1);

        QVERIFY(dbusSpy.count() >= 1);
        verifyPropertiesChangedSignal(dbusSpy, QStringLiteral("DiscoverableTimeout"), value);

        QList<QVariant> adapterArguments = adapterSpy.takeFirst();
        QCOMPARE(adapterArguments.at(0).toUInt(), value);

        QCOMPARE(unit.adapter->discoverableTimeout(), value);
        QCOMPARE(unit.dbusAdapter->discoverableTimeout(), value);

        unit.adapter->setDiscoverableTimeout(originalValue)->waitForFinished();
    }
}

void AdapterTest::setPairableTest()
{
    Q_FOREACH (const AdapterUnit &unit, m_units) {
        QSignalSpy adapterSpy(unit.adapter, SIGNAL(pairableChanged(bool)));
        QSignalSpy dbusSpy(unit.dbusProperties, SIGNAL(PropertiesChanged(QString,QVariantMap,QStringList)));

        bool originalValue = unit.adapter->isPairable();
        bool value = !originalValue;

        unit.adapter->setPairable(value);
        QTRY_COMPARE(adapterSpy.count(), 1);

        QList<QVariant> adapterArguments = adapterSpy.takeFirst();
        QCOMPARE(adapterArguments.at(0).toBool(), value);
        verifyPropertiesChangedSignal(dbusSpy, QStringLiteral("Pairable"), value);

        QCOMPARE(unit.adapter->isPairable(), value);
        QCOMPARE(unit.dbusAdapter->pairable(), value);

        unit.adapter->setPairable(originalValue)->waitForFinished();
    }
}

void AdapterTest::setPairableTimeoutTest()
{
    Q_FOREACH (const AdapterUnit &unit, m_units) {
        QSignalSpy adapterSpy(unit.adapter, SIGNAL(pairableTimeoutChanged(quint32)));
        QSignalSpy dbusSpy(unit.dbusProperties, SIGNAL(PropertiesChanged(QString,QVariantMap,QStringList)));

        quint32 originalValue = unit.adapter->pairableTimeout();
        quint32 value = originalValue + 1;

        unit.adapter->setPairableTimeout(value);
        QTRY_COMPARE(adapterSpy.count(), 1);

        QVERIFY(dbusSpy.count() >= 1);
        verifyPropertiesChangedSignal(dbusSpy, QStringLiteral("PairableTimeout"), value);

        QList<QVariant> adapterArguments = adapterSpy.takeFirst();
        QCOMPARE(adapterArguments.at(0).toUInt(), value);

        QCOMPARE(unit.adapter->pairableTimeout(), value);
        QCOMPARE(unit.dbusAdapter->pairableTimeout(), value);

        unit.adapter->setPairableTimeout(originalValue)->waitForFinished();
    }
}

void AdapterTest::discoveryTest()
{
    // Discovery needs Adapter powered on

    Q_FOREACH (const AdapterUnit &unit, m_units) {
        // Make sure the Adapter is powered on and not discovering
        bool wasPowered = unit.adapter->isPowered();
        unit.adapter->setPowered(true)->waitForFinished();
        if (unit.adapter->isDiscovering()) {
            unit.adapter->stopDiscovery()->waitForFinished();
        }

        QSignalSpy adapterSpy(unit.adapter, SIGNAL(discoveringChanged(bool)));
        QSignalSpy dbusSpy(unit.dbusProperties, SIGNAL(PropertiesChanged(QString,QVariantMap,QStringList)));

        // Start Discovery
        unit.adapter->startDiscovery();
        QTRY_COMPARE(adapterSpy.count(), 1);

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
        QTRY_COMPARE(adapterSpy.count(), 1);

        QVERIFY(dbusSpy.count() >= 1);
        verifyPropertiesChangedSignal(dbusSpy, QStringLiteral("Discovering"), false);

        adapterArguments = adapterSpy.takeFirst();
        QCOMPARE(adapterArguments.at(0).toBool(), false);

        QCOMPARE(unit.adapter->isDiscovering(), false);
        QCOMPARE(unit.dbusAdapter->discovering(), false);

        unit.adapter->setPowered(wasPowered)->waitForFinished();
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    AdapterTest test1(false /*fakeBluezRun*/);
    int res = QTest::qExec(&test1, argc, argv);
    if (res != 0) {
        return res;
    }

    AdapterTest test2(true /*fakeBluezRun*/);
    return QTest::qExec(&test2, argc, argv);
}
