/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * SPDX-FileCopyrightText: 2019 Manuel Weichselbaumer <mincequi@web.de>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef BLUEZQT_LEADVERTISINGMANAGER_H
#define BLUEZQT_LEADVERTISINGMANAGER_H

#include <QObject>

#include "bluezqt_export.h"

#include <memory>

namespace BluezQt
{
class LEAdvertisement;
class PendingCall;

/*!
 * \inmodule BluezQt
 * \class BluezQt::LEAdvertisingManager
 * \inheaderfile BluezQt/LEAdvertisingManager
 * \brief Bluetooth LE advertising manager.
 *
 * The Advertising Manager allows external applications to register Advertisement
 * Data which should be broadcast to devices. Advertisement Data elements must
 * follow the API for LE Advertisement Data.
 *
 * \sa LEAdvertisement
 */
class BLUEZQT_EXPORT LEAdvertisingManager : public QObject
{
    Q_OBJECT

public:
    ~LEAdvertisingManager() override;

    /*!
     * Registers an \a advertisement object to be sent over the LE Advertising
     * channel.
     *
     * The service must be exported under interface LEAdvertisement1.
     *
     * InvalidArguments error indicates invalid or conflicting properties.
     *
     * InvalidLength error indicates that provided data results in too long data packet.
     *
     * The properties of this object are parsed on register, any changes are ignored.
     *
     * If the same object is registered twice it will result in an AlreadyExists error.
     *
     * NotPermitted error indicates that the maximum number of advertisements is reached.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::InvalidArguments
     * \li PendingCall::AlreadyExists
     * \li PendingCall::InvalidLength
     * \li PendingCall::NotPermitted
     * \endlist
     *
     * Returns void pending call.
     */
    PendingCall *registerAdvertisement(LEAdvertisement *advertisement);

    /*!
     * Unregisters \a advertisement.
     *
     * This unregisters an advertisement that has been previously registered.
     * The object path must match the same value that has been used on registration.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::InvalidArguments
     * \li PendingCall::DoesNotExist
     * \endlist
     *
     * Returns void pending call.
     */
    PendingCall *unregisterAdvertisement(LEAdvertisement *advertisement);

private:
    BLUEZQT_NO_EXPORT explicit LEAdvertisingManager(const QString &path, QObject *parent = nullptr);

    std::unique_ptr<class LEAdvertisingManagerPrivate> const d;

    friend class AdapterPrivate;
};

} // namespace BluezQt

#endif // BLUEZQT_LEADVERTISINGMANAGER_H
