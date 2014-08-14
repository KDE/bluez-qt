#ifndef JOBSTEST_H
#define JOBSTEST_H

#include <QObject>

class JobsTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();

    void deleteManagerTest();
    void deleteInitManagerJobTest();
    void deleteInitAdaptersJobTest();
    void deleteObexManagerTest();
    void deleteInitObexManagerJobTest();
    void deleteLoadDeviceJobTest();
};

#endif // JOBSTEST_H
