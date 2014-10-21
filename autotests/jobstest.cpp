#include "jobstest.h"
#include "manager.h"
#include "initmanagerjob.h"
#include "obexmanager.h"
#include "initobexmanagerjob.h"
#include "device.h"

#include <QtTest/QTest>
#include <QtTest/QSignalSpy>
#include <QLoggingCategory>

using namespace QBluez;

void JobsTest::initTestCase()
{
    QLoggingCategory::setFilterRules(QStringLiteral("QBluez=false"));
}

void JobsTest::deleteManagerTest()
{
    Manager *manager = new Manager();
    InitManagerJob *job = manager->init();
    QSignalSpy jobSpy(job, SIGNAL(result(InitManagerJob*)));

    job->start();
    delete manager;

    // Deleting manager while init job is in progress should emit error
    QTRY_COMPARE(jobSpy.count(), 1);
}

void JobsTest::deleteInitManagerJobTest()
{
    Manager *manager = new Manager();
    InitManagerJob *job = manager->init();
    QSignalSpy jobSpy(job, SIGNAL(result(InitManagerJob*)));

    job->start();
    delete job;

    // Deleting the job should emit error
    QTRY_COMPARE(jobSpy.count(), 1);

    delete manager;

    manager = new Manager();
    job = manager->init();
    QSignalSpy jobSpy2(job, SIGNAL(result(InitManagerJob*)));

    job->start();
    job->kill();

    // Killing the job should NOT emit error
    QTRY_COMPARE(jobSpy2.count(), 0);
}

void JobsTest::deleteObexManagerTest()
{
    ObexManager *manager = new ObexManager();
    InitObexManagerJob *job = manager->init();
    QSignalSpy jobSpy(job, SIGNAL(result(InitObexManagerJob*)));

    job->start();
    delete manager;

    // Deleting manager while init job is in progress should emit error
    QTRY_COMPARE(jobSpy.count(), 1);
}

void JobsTest::deleteInitObexManagerJobTest()
{
    ObexManager *manager = new ObexManager();
    InitObexManagerJob *job = manager->init();
    QSignalSpy jobSpy(job, SIGNAL(result(InitObexManagerJob*)));

    job->start();
    delete job;

    // Deleting the job should emit error
    QTRY_COMPARE(jobSpy.count(), 1);

    delete manager;

    manager = new ObexManager();
    job = manager->init();
    QSignalSpy jobSpy2(job, SIGNAL(result(InitObexManagerJob*)));

    job->start();
    job->kill();

    // Killing the job should NOT emit error
    QTRY_COMPARE(jobSpy2.count(), 0);
}

QTEST_MAIN(JobsTest)
