#include "testinterface.h"
#include "fakebluez.h"

TestInterface::TestInterface(FakeBluez *parent)
    : QDBusAbstractAdaptor(parent)
    , m_fakeBluez(parent)
{
}

void TestInterface::runTest(const QString &testName)
{
    m_fakeBluez->runTest(testName);
}
