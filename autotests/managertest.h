#ifndef MANAGERTEST_H
#define MANAGERTEST_H

#include <QObject>

class ManagerTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void bluezNotRunningTest();
    void bluezNotExportingInterfacesTest();
    void bluezEmptyManagedObjectsTest();
    void bluezNoAdaptersTest();

};

#endif // MANAGERTEST_H
