/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * Copyright (C) 2015 David Rosca <nowrep@gmail.com>
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

#ifndef BLUEZQT_INPUT_H
#define BLUEZQT_INPUT_H

#include <QObject>

#include "types.h"
#include "bluezqt_export.h"

namespace BluezQt
{

/**
 * @class BluezQt::Input input.h <BluezQt/Input>
 *
 * %Device input.
 *
 * This class represents an input interface.
 */
class BLUEZQT_EXPORT Input : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ReconnectMode reconnectMode READ reconnectMode NOTIFY reconnectModeChanged)

public:
    /** Reconnect mode. */
    enum ReconnectMode {
        /** Device and host are not required to automatically restore the connection. */
        NoReconnect,
        /** Host restores the connection. */
        HostReconnect,
        /** Device restores the connection. */
        DeviceReconnect,
        /** Device shall attempt to restore the lost connection, but host may also restore the connection. */
        AnyReconnect
    };
    Q_ENUM(ReconnectMode)

    /**
     * Destroys an Input object.
     */
    ~Input();

    /**
     * Returns a shared pointer from this.
     *
     * @return InputPtr
     */
    InputPtr toSharedPtr() const;

    /**
     * Returns the reconnect mode.
     *
     * @return reconnect mode
     */
    ReconnectMode reconnectMode() const;

Q_SIGNALS:
    /**
     * Indicates that input's reconnect mode have changed.
     */
    void reconnectModeChanged(ReconnectMode mode);

private:
    explicit Input(const QString &path, const QVariantMap &properties);

    class InputPrivate *const d;

    friend class InputPrivate;
    friend class DevicePrivate;
};

} // namespace BluezQt

#endif // BLUEZQT_INPUT_H
