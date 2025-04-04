/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * SPDX-FileCopyrightText: 2019 Manuel Weichselbaumer <mincequi@web.de>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef BLUEZQT_MEDIATRANSPORT_H
#define BLUEZQT_MEDIATRANSPORT_H

#include <QObject>

#include "bluezqt_export.h"
#include "mediatypes.h"
#include "tpendingcall.h"

#include <memory>

namespace BluezQt
{
class PendingCall;

/*!
 * \inmodule BluezQt
 * \class BluezQt::MediaTransport
 * \inheaderfile BluezQt/MediaTransport
 * \brief Media transport.
 *
 * This class represents a media transport interface.
 */
class BLUEZQT_EXPORT MediaTransport : public QObject
{
    Q_OBJECT
    /*! \property BluezQt::MediaTransport::state */
    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    /*! \property BluezQt::MediaTransport::volume */
    Q_PROPERTY(quint16 volume READ volume NOTIFY volumeChanged)

public:
    /*!
     * \enum BluezQt::MediaTransport::State
     * \brief Indicates the state of the transport.
     * \value Idle
     * \value Pending
     * \value Active
     */
    enum class State {
        Idle,
        Pending,
        Active,
    };
    Q_ENUM(State)

    ~MediaTransport() override;

    /*!
     * Returns the (audio) configuration of the transport.
     */
    AudioConfiguration audioConfiguration() const;

    /*!
     * Returns the state of the transport.
     */
    State state() const;

    /*!
     * Returns the volume of the transport.
     *
     * The volume is a percentage of the maximum. The value 0x00 corresponds to 0%.
     * The value 0x7F corresponds to 100%. Scaling should be applied to achieve
     * values between these two.
     *
     * The existence of this scale does not impose any
     * restriction on the granularity of the volume control scale on the target.
     *
     * As this command specifies a percentage rather than an absolute dB level
     * the controller should exercise caution when sending this command.
     */
    quint16 volume() const;

    /*!
     * Sets the \a volume of the transport.
     *
     * Only works when the transport was acquired by the sender.
     *
     * The volume of the transport should be within the range [0x00..0x7F] (0-127)
     *
     * Returns void pending call.
     * \since 6.6
     */
    PendingCall *setVolume(quint16 volume);

public Q_SLOTS:
    /*!
     * Acquires the transport file descriptor and the MTU for read
     * and write respectively.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::NotAuthorized
     * \li PendingCall::Failed
     * \endlist
     *
     * Returns <fd, uint16, uint16> pending call.
     */
    TPendingCall<QDBusUnixFileDescriptor, uint16_t, uint16_t> *acquire();

    /*!
     * Acquire transport file descriptor only if the transport
     * is in "pending" state at the time the message is
     * received by BlueZ. Otherwise no request will be sent
     * to the remote device and the function will just fail
     * with org.bluez.Error.NotAvailable.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::NotAuthorized
     * \li PendingCall::Failed
     * \li PendingCall::NotAvailable
     * \endlist
     *
     * Returns <fd, uint16, uint16> pending call.
     */
    TPendingCall<QDBusUnixFileDescriptor, uint16_t, uint16_t> *tryAcquire();

    /*!
     * Releases file descriptor.
     */
    TPendingCall<void> *release();

Q_SIGNALS:
    /*!
     * Indicates that the transport's \a state has changed.
     */
    void stateChanged(State state);

    /*!
     * Indicates that the transport's \a volume has changed.
     */
    void volumeChanged(quint16 volume);

private:
    BLUEZQT_NO_EXPORT explicit MediaTransport(const QString &path, const QVariantMap &properties);

    std::unique_ptr<class MediaTransportPrivate> const d;

    friend class MediaTransportPrivate;
    friend class DevicePrivate;
};

} // namespace BluezQt

#endif
