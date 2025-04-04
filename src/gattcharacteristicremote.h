/*
 * BluezQt - Asynchronous Bluez wrapper library
 *
 * SPDX-FileCopyrightText: 2021 Ivan Podkurkov <podkiva2@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef BLUEZQT_GATTCHARACTERISTICREMOTE_H
#define BLUEZQT_GATTCHARACTERISTICREMOTE_H

#include "bluezqt_export.h"
#include "gattdescriptorremote.h"
#include "types.h"
#include <QList>
#include <QMap>
#include <QObject>
namespace BluezQt
{

class GattServiceRemote;
class PendingCall;

/*!
 * \inmodule BluezQt
 * \class BluezQt::GattCharacteristicRemote
 * \inheaderfile BluezQt/GattCharacteristicRemote
 *
 * \brief Bluetooth LE GATT characteristic.
 *
 * This class represents a Bluetooth LE GATT characteristic for the clients.
 */
class BLUEZQT_EXPORT GattCharacteristicRemote : public QObject
{
    Q_OBJECT
    /*! \property BluezQt::GattCharacteristicRemote::ubi */
    Q_PROPERTY(QString ubi READ ubi CONSTANT)
    /*! \property BluezQt::GattCharacteristicRemote::uuid */
    Q_PROPERTY(QString uuid READ uuid NOTIFY uuidChanged)
    /*! \property BluezQt::GattCharacteristicRemote::value */
    Q_PROPERTY(QByteArray value READ value NOTIFY valueChanged)
    /*! \property BluezQt::GattCharacteristicRemote::writeAcquired */
    Q_PROPERTY(bool writeAcquired READ isWriteAcquired NOTIFY writeAcquiredChanged)
    /*! \property BluezQt::GattCharacteristicRemote::notifyAcquired */
    Q_PROPERTY(bool notifyAcquired READ isNotifyAcquired NOTIFY notifyAcquiredChanged)
    /*! \property BluezQt::GattCharacteristicRemote::notifying */
    Q_PROPERTY(bool notifying READ isNotifying NOTIFY notifyingChanged)
    /*! \property BluezQt::GattCharacteristicRemote::flags */
    Q_PROPERTY(QStringList flags READ flags NOTIFY flagsChanged)
    /*! \property BluezQt::GattCharacteristicRemote::handle */
    Q_PROPERTY(quint16 handle READ handle NOTIFY handleChanged)
    /*! \property BluezQt::GattCharacteristicRemote::MTU */
    Q_PROPERTY(quint16 MTU READ MTU NOTIFY MTUChanged)
    /*! \property BluezQt::GattCharacteristicRemote::service */
    Q_PROPERTY(GattServiceRemotePtr service READ service CONSTANT)
    /*! \property BluezQt::GattCharacteristicRemote::descriptors */
    Q_PROPERTY(QList<GattDescriptorRemotePtr> descriptors READ descriptors NOTIFY descriptorsChanged)

public:
    ~GattCharacteristicRemote() override;

    /*!
     * Returns a shared pointer from this.
     */
    GattCharacteristicRemotePtr toSharedPtr() const;

    /*!
     * Returns the UBI of the GATT characteristic.
     *
     * Example UBI: "/org/bluez/hci0/dev_40_79_6A_0C_39_75"
     */
    QString ubi() const;

    /*!
     * Returns the UUID of the characteristic.
     */
    QString uuid() const;

    /*!
     * Returns the value of the characteristic.
     */
    QByteArray value() const;

    /*!
     * Returns whether writeAcquired() is \c true for the characteristic.
     */
    bool isWriteAcquired() const;

    /*!
     * Returns whether notifyAcquired() is \c true for the characteristic.
     */
    bool isNotifyAcquired() const;

    /*!
     * Returns whether the characteristic is notifying.
     */
    bool isNotifying() const;

    /*!
     * Returns the characteristic's flags.
     */
    QStringList flags() const;

    /*!
     * Returns the characteristic's handle.
     */
    quint16 handle() const;

    /*!
     * Sets the characteristic's \a handle.
     *
     * Returns void pending call.
     */
    PendingCall *setHandle(quint16 handle);

    /*!
     * Returns the characteristic's MTU.
     */
    quint16 MTU() const;

    /*!
     * Returns a service that owns that characteristic.
     */
    GattServiceRemotePtr service() const;

    /*!
     * Returns object paths representing the included
     * services of this service.
     */
    QList<GattDescriptorRemotePtr> descriptors() const;

public Q_SLOTS:
    /*!
     * Read the value of the GATT characteristic.
     *
     * Issues a request to read the value of the characteristic and
     * returns the value if the	operation was successful.
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
     * Write the value of the GATT characteristic.
     *
     * Issues a request to write the value of the characteristic.
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

    /*!
     * Start notifying the value of the GATT characteristic.
     *
     * Starts a notification session from this characteristic if it supports
     * value notifications or indications.
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
    PendingCall *startNotify();

    /*!
     * Stop notifying the value of the GATT characteristic.
     *
     * This method will cancel any previous StartNotify transaction.
     * Note that notifications from a characteristic are shared between
     * sessions thus calling StopNotify will release a single session.
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
    PendingCall *stopNotify();

    /*!
     * Confirmation that value of the characteristic was received.
     *
     * This method doesn't expect a reply so it is just a confirmation
     * that value was received.
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
    PendingCall *confirm();

Q_SIGNALS:
    /*!
     * Indicates that at least one of the \a characteristic's properties has changed.
     */
    void characteristicChanged(GattCharacteristicRemotePtr characteristic);

    /*!
     * Indicates that a new \a descriptor was added (eg. found by connection).
     */
    void gattDescriptorAdded(GattDescriptorRemotePtr descriptor);

    /*!
     * Indicates that the characteristic's \a descriptors list has changed.
     */
    void descriptorsChanged(QList<GattDescriptorRemotePtr> descriptors);

    /*!
     * Indicates that a \a descriptor was removed.
     */
    void gattDescriptorRemoved(GattDescriptorRemotePtr descriptor);

    /*!
     * Indicates that at least one of the \a descriptor properties has changed.
     */
    void gattDescriptorChanged(GattDescriptorRemotePtr descriptor);

    /*!
     * Indicates that the characteristic's \a uuid has changed.
     */
    void uuidChanged(const QString &uuid);

    /*!
     * Indicates that the characteristic's \a value has changed.
     */
    void valueChanged(const QByteArray value);

    /*!
     * Indicates that the characteristic's \a writeAcquired state has changed.
     */
    void writeAcquiredChanged(bool writeAcquired);

    /*!
     * Indicates that the characteristic's \a notifyAcquired state has changed.
     */
    void notifyAcquiredChanged(bool notifyAcquired);

    /*!
     * Indicates that the characteristic's \a notifying state has changed.
     */
    void notifyingChanged(bool notifying);

    /*!
     * Indicates that the characteristic's \a flags has changed.
     */
    void flagsChanged(QStringList flags);

    /*!
     * Indicates that the characteristic's handle has changed.
     */
    void handleChanged(quint16 handle);

    /*!
     * Indicates that the characteristic's \a MTU have changed.
     */
    void MTUChanged(quint16 MTU);

private:
    BLUEZQT_NO_EXPORT explicit GattCharacteristicRemote(const QString &path, const QVariantMap &properties, GattServiceRemotePtr service);

    const std::unique_ptr<class GattCharacteristicRemotePrivate> d;

    friend class DevicePrivate;
    friend class GattServiceRemotePrivate;
    friend class GattCharacteristicRemotePrivate;
    friend class ManagerPrivate;
    friend class Adapter;
};

} // namespace BluezQt

#endif // BLUEZQT_GATTCHARACTERISTICREMOTE_H
