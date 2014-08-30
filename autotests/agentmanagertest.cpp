#include "agentmanagertest.h"
#include "autotests.h"
#include "manager.h"
#include "initmanagerjob.h"
#include "pendingcall.h"

#include <QtTest/QTest>
#include <QtTest/QSignalSpy>
#include <QLoggingCategory>
#include <QDBusObjectPath>

namespace QBluez
{
extern void qbluez_initFakeBluezTestRun();
}

using namespace QBluez;


// TestAgent
TestAgent::TestAgent(QObject *parent)
    : Agent(parent)
    , m_device(0)
    , m_pinRequested(false)
    , m_passkeyRequested(false)
    , m_authorizationRequested(false)
    , m_cancelCalled(false)
    , m_releaseCalled(false)
{
}

QDBusObjectPath TestAgent::objectPath() const
{
    return QDBusObjectPath(QStringLiteral("/testagent"));
}

void TestAgent::requestPinCode(Device *device, const QBluez::Request<QString> &request)
{
    m_device = device;
    m_pinRequested = true;

    request.accept(QString());

}

void TestAgent::displayPinCode(Device *device, const QString &pinCode)
{
    m_device = device;
    m_displayedPinCode = pinCode;
}

void TestAgent::requestPasskey(Device *device, const QBluez::Request<quint32> &request)
{
    m_device = device;
    m_passkeyRequested = true;

    request.accept(0);
}

void TestAgent::displayPasskey(Device *device, const QString &passkey, const QString &entered)
{
    m_device = device;
    m_displayedPasskey = passkey;
    m_enteredPasskey = entered;
}

void TestAgent::requestConfirmation(Device *device, const QString &passkey, const QBluez::Request<> &request)
{
    m_device = device;
    m_requestedPasskey = passkey;

    request.accept();
}

void TestAgent::requestAuthorization(Device *device, const QBluez::Request<> &request)
{
    m_device = device;
    m_authorizationRequested = true;

    request.accept();
}

void TestAgent::authorizeService(Device *device, const QString &uuid, const QBluez::Request<> &request)
{
    m_device = device;
    m_authorizedUuid = uuid;

    request.accept();
}

void TestAgent::cancel()
{
    m_cancelCalled = true;
}

void TestAgent::release()
{
    m_releaseCalled = true;
}

// AgentManagerTest
void AgentManagerTest::initTestCase()
{
    qbluez_initFakeBluezTestRun();
    QLoggingCategory::setFilterRules(QStringLiteral("QBluez=false"));

    FakeBluez::start();
    FakeBluez::runTest(QStringLiteral("bluez-standard"));

    // Create adapter
    QVariantMap adapterProps;
    adapterProps[QStringLiteral("Path")] = QVariant::fromValue(QDBusObjectPath(QStringLiteral("/org/bluez/hci0")));
    adapterProps[QStringLiteral("Address")] = QStringLiteral("1C:E5:C3:BC:94:7E");
    adapterProps[QStringLiteral("Name")] = QStringLiteral("TestAdapter");
    FakeBluez::runAction(QStringLiteral("devicemanager"), QStringLiteral("create-adapter"), adapterProps);

    // Create device
    m_device = QDBusObjectPath(QStringLiteral("/org/bluez/hci0/dev_40_79_6A_0C_39_75"));
    QVariantMap deviceProps;
    deviceProps[QStringLiteral("Path")] = QVariant::fromValue(m_device);
    deviceProps[QStringLiteral("Adapter")] = adapterProps.value(QStringLiteral("Path"));
    deviceProps[QStringLiteral("Address")] = QStringLiteral("40:79:6A:0C:39:75");
    deviceProps[QStringLiteral("Name")] = QStringLiteral("TestDevice");
    FakeBluez::runAction(QStringLiteral("devicemanager"), QStringLiteral("create-device"), deviceProps);

    Manager *manager = new Manager(this);
    InitManagerJob *job = manager->init();
    job->exec();

    QVERIFY(!job->error());
    QCOMPARE(manager->adapters().count(), 1);
    QCOMPARE(manager->devices().count(), 1);

    m_agent = new TestAgent(this);
    manager->registerAgent(m_agent)->waitForFinished();
}

void AgentManagerTest::cleanupTestCase()
{
    FakeBluez::stop();
}

void AgentManagerTest::requestPinCodeTest()
{
    QCOMPARE(m_agent->m_pinRequested, false);

    QVariantMap props;
    props.insert(QStringLiteral("Device"), QVariant::fromValue(m_device));
    FakeBluez::runAction(QStringLiteral("agentmanager"), QStringLiteral("request-pincode"), props);

    QCOMPARE(m_agent->m_pinRequested, true);
    QCOMPARE(m_agent->m_device->name(), QStringLiteral("TestDevice"));
}

void AgentManagerTest::displayPinCodeTest()
{
    QCOMPARE(m_agent->m_displayedPinCode, QString());

    QVariantMap props;
    props.insert(QStringLiteral("Device"), QVariant::fromValue(m_device));
    props.insert(QStringLiteral("PinCode"), QStringLiteral("123456"));
    FakeBluez::runAction(QStringLiteral("agentmanager"), QStringLiteral("display-pincode"), props);

    QCOMPARE(m_agent->m_displayedPinCode, QStringLiteral("123456"));
    QCOMPARE(m_agent->m_device->name(), QStringLiteral("TestDevice"));
}

void AgentManagerTest::requestPasskeyTest()
{
    QCOMPARE(m_agent->m_passkeyRequested, false);

    QVariantMap props;
    props.insert(QStringLiteral("Device"), QVariant::fromValue(m_device));
    FakeBluez::runAction(QStringLiteral("agentmanager"), QStringLiteral("request-passkey"), props);

    QCOMPARE(m_agent->m_passkeyRequested, true);
    QCOMPARE(m_agent->m_device->name(), QStringLiteral("TestDevice"));
}

void AgentManagerTest::displayPasskeyTest()
{
    QCOMPARE(m_agent->m_displayedPasskey, QString());
    QCOMPARE(m_agent->m_enteredPasskey, QString());

    QVariantMap props;
    props.insert(QStringLiteral("Device"), QVariant::fromValue(m_device));
    props.insert(QStringLiteral("Passkey"), QVariant::fromValue((quint32) 654321));
    props.insert(QStringLiteral("EnteredPasskey"), QVariant::fromValue((quint16) 43));
    FakeBluez::runAction(QStringLiteral("agentmanager"), QStringLiteral("display-passkey"), props);

    QCOMPARE(m_agent->m_displayedPasskey, QStringLiteral("654321"));
    QCOMPARE(m_agent->m_enteredPasskey, QStringLiteral("43"));
    QCOMPARE(m_agent->m_device->name(), QStringLiteral("TestDevice"));
}

void AgentManagerTest::requestConfirmationTest()
{
    QCOMPARE(m_agent->m_requestedPasskey, QString());

    QVariantMap props;
    props.insert(QStringLiteral("Device"), QVariant::fromValue(m_device));
    props.insert(QStringLiteral("Passkey"), QVariant::fromValue((quint32) 12));
    FakeBluez::runAction(QStringLiteral("agentmanager"), QStringLiteral("request-confirmation"), props);

    QCOMPARE(m_agent->m_requestedPasskey, QStringLiteral("000012"));
    QCOMPARE(m_agent->m_device->name(), QStringLiteral("TestDevice"));
}

void AgentManagerTest::requestAuthorizationTest()
{
    QCOMPARE(m_agent->m_authorizationRequested, false);

    QVariantMap props;
    props.insert(QStringLiteral("Device"), QVariant::fromValue(m_device));
    FakeBluez::runAction(QStringLiteral("agentmanager"), QStringLiteral("request-authorization"), props);

    QCOMPARE(m_agent->m_authorizationRequested, true);
    QCOMPARE(m_agent->m_device->name(), QStringLiteral("TestDevice"));
}

void AgentManagerTest::authorizeServiceTest()
{
    QCOMPARE(m_agent->m_authorizedUuid, QString());

    QVariantMap props;
    props.insert(QStringLiteral("Device"), QVariant::fromValue(m_device));
    props.insert(QStringLiteral("UUID"), Services::ObexFileTransfer);
    FakeBluez::runAction(QStringLiteral("agentmanager"), QStringLiteral("authorize-service"), props);

    QCOMPARE(m_agent->m_authorizedUuid, Services::ObexFileTransfer);
    QCOMPARE(m_agent->m_device->name(), QStringLiteral("TestDevice"));
}

void AgentManagerTest::cancelTest()
{
    QCOMPARE(m_agent->m_cancelCalled, false);

    FakeBluez::runAction(QStringLiteral("agentmanager"), QStringLiteral("cancel"));

    QCOMPARE(m_agent->m_cancelCalled, true);
    QCOMPARE(m_agent->m_device->name(), QStringLiteral("TestDevice"));
}

void AgentManagerTest::releaseTest()
{
    QCOMPARE(m_agent->m_releaseCalled, false);

    FakeBluez::runAction(QStringLiteral("agentmanager"), QStringLiteral("release"));

    QCOMPARE(m_agent->m_releaseCalled, true);
    QCOMPARE(m_agent->m_device->name(), QStringLiteral("TestDevice"));
}

QTEST_MAIN(AgentManagerTest)
