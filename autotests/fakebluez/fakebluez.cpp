#include "fakebluez.h"
#include "testinterface.h"
#include "objectmanager.h"
#include "agentmanager.h"

#include <QDBusConnection>

FakeBluez::FakeBluez(QObject *parent)
    : QObject(parent)
    , m_testInterface(new TestInterface(this))
    , m_objectManager(0)
    , m_agentManager(0)
{
    QDBusConnection::sessionBus().registerObject(QStringLiteral("/"), this);
}

void FakeBluez::runTest(const QString &testName)
{
    if (testName == QLatin1String("bluez-not-exporting-interfaces")) {
        // We do nothing here
    } else if (testName == QLatin1String("bluez-empty-managed-objects")) {
        runBluezEmptyManagedObjectsTest();
    } else if (testName == QLatin1String("bluez-no-adapters")) {
        runBluezNoAdaptersTest();
    }
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
