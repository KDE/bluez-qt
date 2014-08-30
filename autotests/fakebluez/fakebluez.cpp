#include "fakebluez.h"
#include "testinterface.h"
#include "objectmanager.h"
#include "agentmanager.h"
#include "devicemanager.h"

#include <QTimer>
#include <QDBusConnection>

FakeBluez::FakeBluez(QObject *parent)
    : QObject(parent)
    , m_testInterface(new TestInterface(this))
    , m_objectManager(0)
    , m_agentManager(0)
    , m_deviceManager(0)
{
    QDBusConnection::sessionBus().registerObject(QStringLiteral("/"), this);
}

void FakeBluez::runTest(const QString &testName)
{
    if (testName == QLatin1String("bluez-not-exporting-interfaces")) {
        runBluezNotExportingInterfacesTest();
    } else if (testName == QLatin1String("bluez-empty-managed-objects")) {
        runBluezEmptyManagedObjectsTest();
    } else if (testName == QLatin1String("bluez-no-adapters")) {
        runBluezNoAdaptersTest();
    } else if (testName == QLatin1String("bluez-standard")) {
        runBluezStandardTest();
    }
}

void FakeBluez::runAction(const QString &object, const QString &actionName, const QVariantMap &properties)
{
    m_actionObject = object;
    m_actionName = actionName;
    m_actionProperties = properties;

    QTimer::singleShot(0, this, SLOT(doRunAction()));
}

void FakeBluez::doRunAction()
{
    if (m_actionObject == QLatin1String("agentmanager")) {
        m_agentManager->runAction(m_actionName, m_actionProperties);
    } else if (m_actionObject == QLatin1String("devicemanager")) {
        m_deviceManager->runAction(m_actionName, m_actionProperties);
    }

    m_testInterface->emitActionFinished();
}

void FakeBluez::clear()
{
    // Everything is parented to ObjectManager
    delete m_objectManager;
}

void FakeBluez::createObjectManager()
{
    m_objectManager = new ObjectManager(this);
}

void FakeBluez::createAgentManager()
{
    m_agentManager = new AgentManager(m_objectManager);
    m_objectManager->addObject(m_agentManager);
}

void FakeBluez::createDeviceManager()
{
    m_deviceManager = new DeviceManager(m_objectManager);
}

void FakeBluez::runBluezNotExportingInterfacesTest()
{
    clear();
}

void FakeBluez::runBluezEmptyManagedObjectsTest()
{
    clear();
    createObjectManager();
}

void FakeBluez::runBluezNoAdaptersTest()
{
    clear();
    createObjectManager();
    createAgentManager();
}

void FakeBluez::runBluezStandardTest()
{
    clear();
    createObjectManager();
    createDeviceManager();
    createAgentManager();
}
