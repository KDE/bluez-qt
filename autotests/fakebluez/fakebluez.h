#ifndef FAKEBLUEZ_H
#define FAKEBLUEZ_H

#include <QObject>
#include <QVariantMap>

class TestInterface;
class ObjectManager;
class AgentManager;

class FakeBluez : public QObject
{
    Q_OBJECT

public:
    explicit FakeBluez(QObject *parent = 0);

    void runTest(const QString &testName);
    void runAction(const QString &object, const QString &actionName, const QVariantMap &properties);

private Q_SLOTS:
    void doRunAction();

private:
    void clear();
    void createObjectManager();
    void createAgentManager();

    void runBluezNotExportingInterfacesTest();
    void runBluezEmptyManagedObjectsTest();
    void runBluezNoAdaptersTest();
    void runBluezAgentManagerTest();

    TestInterface *m_testInterface;
    ObjectManager *m_objectManager;
    AgentManager *m_agentManager;

    QString m_actionObject;
    QString m_actionName;
    QVariantMap m_actionProperties;
};

#endif // FAKEBLUEZ_H
