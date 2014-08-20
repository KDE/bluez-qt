#ifndef FAKEBLUEZ_H
#define FAKEBLUEZ_H

#include <QObject>

class TestInterface;
class ObjectManager;
class AgentManager;

class FakeBluez : public QObject
{
public:
    explicit FakeBluez(QObject *parent = 0);

    void runTest(const QString &testName);

private:
    void clear();
    void createObjectManager();
    void createAgentManager();

    void runBluezEmptyManagedObjectsTest();
    void runBluezNoAdaptersTest();

    TestInterface *m_testInterface;
    ObjectManager *m_objectManager;
    AgentManager *m_agentManager;
};

#endif // FAKEBLUEZ_H
