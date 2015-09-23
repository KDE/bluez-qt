/*
 * Copyright (C) 2014-2015 David Rosca <nowrep@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "jobstest.h"
#include "manager.h"
#include "initmanagerjob.h"
#include "obexmanager.h"
#include "initobexmanagerjob.h"
#include "device.h"

#include <QtTest/QTest>
#include <QtTest/QSignalSpy>

using namespace BluezQt;

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
