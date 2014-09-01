#ifndef OBEXMANAGERTEST_H
#define OBEXMANAGERTEST_H

#include <QObject>

class ObexManagerTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void obexNotRunningTest();
    void obexNotExportingInterfacesTest();
    void obexEmptyManagedObjectsTest();
    void obexNoClientTest();
    void obexNoAgentManagerTest();
    void obexServiceOkTest();

};

#endif // OBEXMANAGERTEST_H
