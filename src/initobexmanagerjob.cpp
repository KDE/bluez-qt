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

#include "initobexmanagerjob.h"
#include "obexmanager.h"
#include "obexmanager_p.h"
#include "debug.h"

namespace BluezQt
{

class InitObexManagerJobPrivate : public QObject
{
public:
    explicit InitObexManagerJobPrivate(InitObexManagerJob *q, ObexManager *manager);

    void doStart();
    void initError(const QString &errorText);
    void initFinished();

    InitObexManagerJob *q;
    ObexManager *m_manager;
};

InitObexManagerJobPrivate::InitObexManagerJobPrivate(InitObexManagerJob *q, ObexManager *manager)
    : QObject(q)
    , q(q)
    , m_manager(manager)
{
}

void InitObexManagerJobPrivate::doStart()
{
    if (m_manager->d->m_initialized) {
        qCWarning(BLUEZQT) << "InitObexManagerJob: ObexManager already initialized!";
        q->emitResult();
        return;
    }

    connect(m_manager->d, &ObexManagerPrivate::initError, this, &InitObexManagerJobPrivate::initError);
    connect(m_manager->d, &ObexManagerPrivate::initFinished, this, &InitObexManagerJobPrivate::initFinished);

    m_manager->d->init();
}

void InitObexManagerJobPrivate::initError(const QString &errorText)
{
    qCWarning(BLUEZQT) << "InitObexManagerJob Error:" << errorText;

    q->setError(InitObexManagerJob::UserDefinedError);
    q->setErrorText(errorText);
    q->emitResult();
}

void InitObexManagerJobPrivate::initFinished()
{
    q->emitResult();
}

InitObexManagerJob::InitObexManagerJob(ObexManager *manager)
    : Job(manager)
    , d(new InitObexManagerJobPrivate(this, manager))
{
}

InitObexManagerJob::~InitObexManagerJob()
{
    if (isRunning()) {
        qCWarning(BLUEZQT) << "InitObexManagerJob Error: Job was deleted before finished!";

        setError(UserDefinedError);
        setErrorText(QStringLiteral("Job was deleted before finished."));
        emitResult();
    }
    delete d;
}

ObexManager *InitObexManagerJob::manager() const
{
    return d->m_manager;
}

void InitObexManagerJob::doStart()
{
    d->doStart();
}

void InitObexManagerJob::doEmitResult()
{
    Q_EMIT result(this);
}

} // namespace BluezQt
