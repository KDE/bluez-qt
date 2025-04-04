/*
 * BluezQt - Asynchronous Bluez wrapper library
 *
 * SPDX-FileCopyrightText: 2021 Ivan Podkurkov <podkiva2@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef BLUEZQT_GATTSERVICEREMOTE_H
#define BLUEZQT_GATTSERVICEREMOTE_H

#include <QObject>
#include <QMap>
#include <QDBusObjectPath>

#include "types.h"
#include "bluezqt_export.h"

namespace BluezQt
{

class Device;
class PendingCall;

/*!
 * \inmodule BluezQt
 * \class BluezQt::GattService
 * \inheaderfile BluezQt/GattService
 * \brief Bluetooth LE GATT service.
 *
 * This class represents a Bluetooth LE GATT service.
 */
class BLUEZQT_EXPORT GattServiceRemote : public QObject
{
    Q_OBJECT

    /*! \property BluezQt::GattService::uuid */
    Q_PROPERTY(QString uuid READ uuid NOTIFY uuidChanged)
    /*! \property BluezQt::GattService::primary */
    Q_PROPERTY(bool primary READ isPrimary NOTIFY primaryChanged)
    /*! \property BluezQt::GattService::device */
    Q_PROPERTY(DevicePtr device READ device CONSTANT)
    /*! \property BluezQt::GattService::includes */
    Q_PROPERTY(QList<QDBusObjectPath> includes READ includes NOTIFY includesChanged)
    /*! \property BluezQt::GattService::handle */
    Q_PROPERTY(quint16 handle READ handle WRITE setHandle NOTIFY handleChanged)
    /*! \property BluezQt::GattService::characteristics */
    Q_PROPERTY(QList<GattCharacteristicRemotePtr> characteristics READ characteristics NOTIFY characteristicsChanged)

public:
    ~GattServiceRemote() override;

    /*!
     * Returns a shared pointer from this.
     */
    GattServiceRemotePtr toSharedPtr() const;

    /*!
     * Returns the UBI of the gatt service.
     *
     * Example UBI: "/org/bluez/hci0/dev_40_79_6A_0C_39_75/service01"
     */
    QString ubi() const;

    /*!
     * Returns the UUID of the service.
     */
    QString uuid() const;

    /*!
     * Returns whether the service is primary.
     */
    bool isPrimary() const;

    /*!
     * Returns the device that has this service.
     */
    DevicePtr device() const;

    /*!
     * Returns object paths representing the included
     * services of this service.
     */
    QList<QDBusObjectPath> includes() const;

    /*!
     * Returns service handle.
     *
     * Returns qint16 service handle
     */
    quint16 handle() const;

    /*!
     * Sets the service \a handle.
     *
     * Returns void pending call.
     */
    PendingCall *setHandle(quint16 handle);

    /*!
     * Returns object paths representing the included
     * services of this service.
     */
    QList<GattCharacteristicRemotePtr> characteristics() const;

Q_SIGNALS:
    /*!
     * Indicates that at least one of the \a service's properties has changed.
     */
    void serviceChanged(GattServiceRemotePtr service);

    /*!
     * Indicates that a new \a characteristic was added (eg. found by connection).
     */
    void gattCharacteristicAdded(GattCharacteristicRemotePtr characteristic);

    /*!
     * Indicates that the service \a characteristics list has changed.
     */
    void characteristicsChanged(QList<GattCharacteristicRemotePtr> characteristics);

    /*!
     * Indicates that a \a characteristic was removed.
     */
    void gattCharacteristicRemoved(GattCharacteristicRemotePtr characteristic);

    /*!
     * Indicates that at least one of the \a characteristic's properties has changed.
     */
    void gattCharacteristicChanged(GattCharacteristicRemotePtr characteristic);

    /*!
     * Indicates that the service's \a uuid has changed.
     */
    void uuidChanged(const QString &uuid);

    /*!
     * Indicates that the services's \a primary state has changed.
     */
    void primaryChanged(bool primary);

    /*!
     * Indicates that the services's \a handle has changed.
     */
    void handleChanged(quint16 handle);

    /*!
     * Indicates that the object paths representing the included
     * services \a includes have changed.
     */
    void includesChanged(const QList<QDBusObjectPath> &includes);

private:
    BLUEZQT_NO_EXPORT explicit GattServiceRemote(const QString &path, const QVariantMap &properties, DevicePtr device);

    const std::unique_ptr<class GattServiceRemotePrivate> d;

    friend class GattServiceRemotePrivate;
    friend class DevicePrivate;
    friend class ManagerPrivate;
    friend class Adapter;
};

} // namespace BluezQt

#endif // BLUEZQT_GATTSERVICEREMOTE_H
