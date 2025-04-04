/*
 * BluezQt - Asynchronous Bluez wrapper library
 *
 * SPDX-FileCopyrightText: 2021 Ivan Podkurkov <podkiva2@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef BLUEZQT_GATTDESCRIPTORREMOTE_H
#define BLUEZQT_GATTDESCRIPTORREMOTE_H

#include <QObject>
#include <QMap>

#include "types.h"
#include "bluezqt_export.h"

namespace BluezQt
{

class GattCharacteristicRemote;
class PendingCall;

/*!
 * \inmodule BluezQt
 * \class BluezQt::GattDescriptorRemote
 * \inheaderfile BluezQt/GattDescriptorRemote
 * \brief Bluetooth LE GATT descriptor.
 *
 * This class represents a Bluetooth LE GATT descriptor for the clients.
 */
class BLUEZQT_EXPORT GattDescriptorRemote : public QObject
{
    Q_OBJECT

    /*! \property BluezQt::GattDescriptorRemote::ubi */
    Q_PROPERTY(QString ubi READ ubi CONSTANT)
    /*! \property BluezQt::GattDescriptorRemote::uuid */
    Q_PROPERTY(QString uuid READ uuid NOTIFY uuidChanged)
    /*! \property BluezQt::GattDescriptorRemote::value */
    Q_PROPERTY(QByteArray value READ value NOTIFY valueChanged)
    /*! \property BluezQt::GattDescriptorRemote::flags */
    Q_PROPERTY(QStringList flags READ flags NOTIFY flagsChanged)
    /*! \property BluezQt::GattDescriptorRemote::handle */
    Q_PROPERTY(quint16 handle READ handle NOTIFY handleChanged)
    /*! \property BluezQt::GattDescriptorRemote::characteristic */
    Q_PROPERTY(GattCharacteristicRemotePtr characteristic READ characteristic CONSTANT)

public:
    ~GattDescriptorRemote() override;

    /*!
     * Returns a shared pointer from this.
     */
    GattDescriptorRemotePtr toSharedPtr() const;

    /*!
     * Returns the UBI of the GATT descriptor.
     *
     * Example UBI: "/org/bluez/hci0/dev_40_79_6A_0C_39_75"
     */
    QString ubi() const;

    /*!
     * Returns the uuid of the descriptor.
     */
    QString uuid() const;

    /*!
     * Returns the value of the descriptor.
     */
    QByteArray value() const;

    /*!
     * Returns flags the descriptor.
     */
    QStringList flags() const;

    /*!
     * Returns descriptor handle.
     */
    quint16 handle() const;

    /*!
     * Sets the descriptor \a handle.
     *
     * Returns void pending call.
     */
    PendingCall *setHandle(quint16 handle);

    /*!
     * Returns the characteristic that owns that descriptor.
     */
    GattCharacteristicRemotePtr characteristic() const;

public Q_SLOTS:
    /*!
     * Read the value of the GATT descriptor.
     *
     * Issues a request to read the value of the descriptor and
     * returns the value if the operation was successful.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::NotReady
     * \li PendingCall::Failed
     * \li PendingCall::InProgress
     * \li PendingCall::AlreadyConnected
     * \endlist
     *
     * Returns QByteArray pending call.
     */
    PendingCall *readValue(const QVariantMap &options);

    /*!
     * Write the value of the GATT descriptor.
     *
     * Issues a request to write the value of the descriptor.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::NotReady
     * \li PendingCall::Failed
     * \li PendingCall::InProgress
     * \li PendingCall::AlreadyConnected
     * \endlist
     *
     * Returns void pending call.
     */
    PendingCall *writeValue(const QByteArray &value, const QVariantMap &options);

Q_SIGNALS:
    /*!
     * Indicates that at least one of the descriptors's properties has changed.
     */
    void descriptorChanged(GattDescriptorRemotePtr descriptor);

    /*!
     * Indicates that descriptor's \a uuid has changed.
     */
    void uuidChanged(const QString &uuid);

    /*!
     * Indicates that descriptor's \a value has changed.
     */
    void valueChanged(const QByteArray value);

    /*!
     * Indicates that descriptor's \a flags have changed.
     */
    void flagsChanged(QStringList flags);

    /*!
     * Indicates that descriptor's \a handle has changed.
     */
    void handleChanged(quint16 handle);

private:
    BLUEZQT_NO_EXPORT explicit GattDescriptorRemote(const QString &path, const QVariantMap &properties, GattCharacteristicRemotePtr characteristic);

    const std::unique_ptr<class GattDescriptorRemotePrivate> d;

    friend class DevicePrivate;
    friend class GattServiceRemotePrivate;
    friend class GattCharacteristicRemotePrivate;
    friend class GattDescriptorRemotePrivate;
    friend class ManagerPrivate;
    friend class Adapter;
};

}  // namespace BluezQt

#endif // BLUEZQT_GATTDESCRIPTORREMOTE_H
