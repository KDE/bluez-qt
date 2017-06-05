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

#ifndef BLUEZQT_OBEXSESSION_H
#define BLUEZQT_OBEXSESSION_H

#include <QObject>

#include "types.h"
#include "bluezqt_export.h"

class QDBusObjectPath;

namespace BluezQt
{

class PendingCall;

/**
 * @class BluezQt::ObexSession obexsession.h <BluezQt/ObexSession>
 *
 * OBEX session.
 *
 * This class represents an OBEX session.
 */
class BLUEZQT_EXPORT ObexSession : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString source READ source)
    Q_PROPERTY(QString destination READ destination)
    Q_PROPERTY(quint8 channel READ channel)
    Q_PROPERTY(QString target READ target)
    Q_PROPERTY(QString root READ root)

public:
    /**
     * Destroys an ObexSession object.
     */
    ~ObexSession();

    /**
     * Returns a shared pointer from this.
     *
     * @return ObexSessionPtr
     */
    ObexSessionPtr toSharedPtr() const;

    /**
     * D-Bus object path of the session.
     *
     * @return object path of session
     */
    QDBusObjectPath objectPath() const;

    /**
     * Returns address of the Bluetooth adapter.
     *
     * @see Manager::adapterForAddress() const
     *
     * @return address of adapter
     */
    QString source() const;

    /**
     * Returns address of the Bluetooth device.
     *
     * @see Manager::deviceForAddress() const
     *
     * @return address of device
     */
    QString destination() const;

    /**
     * Returns the Bluetooth channel.
     *
     * @return channel
     */
    quint8 channel() const;

    /**
     * Returns the target UUID.
     *
     * @return target UUID
     */
    QString target() const;

    /**
     * Returns the root path.
     *
     * @return root path
     */
    QString root() const;

    /**
     * Returns the remote device capabilities.
     *
     * Possible errors: PendingCall::NotSupported, PendingCall::Failed
     *
     * @return QString pending call
     */
    PendingCall *getCapabilities();

private:
    explicit ObexSession(const QString &path, const QVariantMap &properties);

    class ObexSessionPrivate *const d;

    friend class ObexSessionPrivate;
    friend class ObexManagerPrivate;
};

} // namespace BluezQt

#endif // BLUEZQT_OBEXSESSION_H
