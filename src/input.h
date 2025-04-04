/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * SPDX-FileCopyrightText: 2015 David Rosca <nowrep@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef BLUEZQT_INPUT_H
#define BLUEZQT_INPUT_H

#include <QObject>

#include "bluezqt_export.h"
#include "types.h"

#include <memory>

namespace BluezQt
{
/*!
 * \inmodule BluezQt
 * \class BluezQt::Input
 * \inheaderfile BluezQt/Input
 * \brief Device input.
 *
 * This class represents an input interface.
 */
class BLUEZQT_EXPORT Input : public QObject
{
    Q_OBJECT
    /*! \property BluezQt::Input::reconnectMode */
    Q_PROPERTY(ReconnectMode reconnectMode READ reconnectMode NOTIFY reconnectModeChanged)

public:
    /*!
     * \enum BluezQt::Input::ReconnectMode
     * \value NoReconnect
     *        Device and host are not required to automatically restore the connection.
     * \value HostReconnect
     *        Host restores the connection.
     * \value DeviceReconnect
     *        Device restores the connection
     * \value AnyReconnect
     *        Device shall attempt to restore the lost connection,
     *        but host may also restore the connection.
     */
    enum ReconnectMode {
        NoReconnect,
        HostReconnect,
        DeviceReconnect,
        AnyReconnect,
    };
    Q_ENUM(ReconnectMode)

    ~Input() override;

    /*!
     * Returns a shared pointer from this.
     */
    InputPtr toSharedPtr() const;

    /*!
     * Returns the reconnect mode.
     */
    ReconnectMode reconnectMode() const;

Q_SIGNALS:
    /*!
     * Indicates that the input's reconnect \a mode has changed.
     */
    void reconnectModeChanged(ReconnectMode mode);

private:
    BLUEZQT_NO_EXPORT explicit Input(const QString &path, const QVariantMap &properties);

    std::unique_ptr<class InputPrivate> const d;

    friend class InputPrivate;
    friend class DevicePrivate;
};

} // namespace BluezQt

#endif // BLUEZQT_INPUT_H
