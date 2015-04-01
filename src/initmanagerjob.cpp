/*
 * BluezQt - Asynchronous Bluez wrapper library
 *
 * Copyright (C) 2014 David Rosca <nowrep@gmail.com>
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

#include "initmanagerjob.h"
#include "manager.h"
#include "manager_p.h"
#include "debug.h"

namespace BluezQt
{

class InitManagerJobPrivate : public QObject
{
public:
    explicit InitManagerJobPrivate(InitManagerJob *q, Manager *manager);

    void doStart();
    void initError(const QString &errorText);
    void initFinished();

    InitManagerJob *q;
    Manager *m_manager;
};

InitManagerJobPrivate::InitManagerJobPrivate(InitManagerJob *q, Manager *manager)
    : QObject(q)
    , q(q)
    , m_manager(manager)
{
}

void InitManagerJobPrivate::doStart()
{
    if (m_manager->d->m_initialized) {
        qCWarning(BLUEZQT) << "InitManagerJob: Manager already initialized!";
        q->emitResult();
        return;
    }

    m_manager->d->init();
    connect(m_manager->d, &ManagerPrivate::initError, this, &InitManagerJobPrivate::initError);
    connect(m_manager->d, &ManagerPrivate::initFinished, this, &InitManagerJobPrivate::initFinished);
}

void InitManagerJobPrivate::initError(const QString &errorText)
{
    qCWarning(BLUEZQT) << "GetManagerJob Error:" << errorText;

    q->setError(InitManagerJob::UserDefinedError);
    q->setErrorText(errorText);
    q->emitResult();
}

void InitManagerJobPrivate::initFinished()
{
    q->emitResult();
}

InitManagerJob::InitManagerJob(Manager *manager)
    : Job(manager)
    , d(new InitManagerJobPrivate(this, manager))
{
}

InitManagerJob::~InitManagerJob()
{
    if (isRunning()) {
        qCWarning(BLUEZQT) << "InitManagerJob Error: Job was deleted before finished!";

        setError(UserDefinedError);
        setErrorText(QStringLiteral("Job was deleted before finished."));
        emitResult();
    }
    delete d;
}

Manager *InitManagerJob::manager() const
{
    return d->m_manager;
}

void InitManagerJob::doStart()
{
    d->doStart();
}

void InitManagerJob::doEmitResult()
{
    Q_EMIT result(this);
}

} // namespace BluezQt
