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

#ifndef BLUEZQT_INITMANAGERJOB_H
#define BLUEZQT_INITMANAGERJOB_H

#include <QObject>

#include "job.h"
#include "bluezqt_export.h"

namespace BluezQt
{

class Manager;

/**
 * @class BluezQt::InitManagerJob initmanagerjob.h <BluezQt/InitManagerJob>
 *
 * Init manager job.
 *
 * This class represents a job that initializes Manager.
 */
class BLUEZQT_EXPORT InitManagerJob : public Job
{
    Q_OBJECT
    Q_PROPERTY(Manager* manager READ manager)

public:
    /**
      * Destroys an InitManagerJob object.
      */
    ~InitManagerJob() override;

    /**
     * Returns a manager that is being initialized.
     *
     * @return manager
     */
    Manager *manager() const;

Q_SIGNALS:
    /**
     * Indicates that the job have finished.
     */
    void result(InitManagerJob *job);

private:
    explicit InitManagerJob(Manager *manager);

    void doStart() override;
    void doEmitResult() override;

    class InitManagerJobPrivate *const d;

    friend class InitManagerJobPrivate;
    friend class Manager;
};

} // namespace BluezQt

#endif // BLUEZQT_INITMANAGERJOB_H
