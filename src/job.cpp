/*
 * BluezQt - Asynchronous Bluez wrapper library
 *
 * Copyright (C) 2014 Alejandro Fiestas Olivares <afiestas@kde.org>
 *               2014 David Rosca <nowrep@gmail.com>
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

#include "job.h"
#include "job_p.h"

#include <QEventLoop>

namespace BluezQt
{

JobPrivate::JobPrivate()
{
    eventLoop = Q_NULLPTR;
    error = Job::NoError;
    running = false;
    finished = false;
    killed = false;
}

Job::Job(QObject *parent)
    : QObject(parent)
    , d_ptr(new JobPrivate)
{
    d_ptr->q_ptr = this;
}

Job::~Job()
{
    delete d_ptr;
}

void Job::start()
{
    d_func()->running = true;
    QMetaObject::invokeMethod(this, "doStart", Qt::QueuedConnection);
}

void Job::kill()
{
    Q_D(Job);
    Q_ASSERT(!d->eventLoop);

    d->running = false;
    d->finished = true;
    d->killed = true;
    deleteLater();
}

void Job::emitResult()
{
    Q_D(Job);

    if (d->killed) {
        return;
    }

    if (d->eventLoop) {
        d->eventLoop->quit();
    }

    d->running = false;
    d->finished = true;
    doEmitResult();
    deleteLater();
}

int Job::error() const
{
    return d_func()->error;
}

QString Job::errorText() const
{
    return d_func()->errorText;
}

bool Job::isRunning() const
{
    return d_func()->running;
}

bool Job::isFinished() const
{
    return d_func()->finished;
}

void Job::setError(int errorCode)
{
    d_func()->error = errorCode;
}

void Job::setErrorText(const QString &errorText)
{
    d_func()->errorText = errorText;
}

bool Job::exec()
{
    Q_D(Job);

    Q_ASSERT(!d->eventLoop);

    QEventLoop loop(this);
    d->eventLoop = &loop;

    start();
    d->eventLoop->exec(QEventLoop::ExcludeUserInputEvents);
    d->running = false;
    d->finished = true;

    return d->error == NoError;
}

} // namespace BluezQt
