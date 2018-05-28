/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
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

#ifndef BLUEZQT_INITOBEXMANAGERJOB_H
#define BLUEZQT_INITOBEXMANAGERJOB_H

#include <QObject>

#include "job.h"
#include "bluezqt_export.h"

namespace BluezQt
{

class ObexManager;

/**
 * @class BluezQt::InitObexManagerJob initobexmanagerjob.h <BluezQt/InitObexManagerJob>
 *
 * Init obex manager job.
 *
 * This class represents a job that initializes ObexManager.
 */
class BLUEZQT_EXPORT InitObexManagerJob : public Job
{
    Q_OBJECT
    Q_PROPERTY(ObexManager* manager READ manager)

public:
    /**
      * Destroys an InitObexManagerJob object.
      */
    ~InitObexManagerJob() override;

    /**
     * Returns a manager that is being initialized.
     *
     * @return manager
     */
    ObexManager *manager() const;

Q_SIGNALS:
    /**
     * Indicates that the job have finished.
     */
    void result(InitObexManagerJob *job);

private:
    explicit InitObexManagerJob(ObexManager *manager);

    void doStart() override;
    void doEmitResult() override;

    class InitObexManagerJobPrivate *const d;

    friend class InitObexManagerJobPrivate;
    friend class ObexManager;
};

} // namespace BluezQt

#endif // BLUEZQT_INITOBEXMANAGERJOB_H
