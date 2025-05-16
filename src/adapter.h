/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * SPDX-FileCopyrightText: 2014-2015 David Rosca <nowrep@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef BLUEZQT_ADAPTER_H
#define BLUEZQT_ADAPTER_H

#include <QList>
#include <QObject>
#include <QStringList>

#include "bluezqt_export.h"
#include "types.h"

#include <memory>

namespace BluezQt
{
class Device;
class PendingCall;

/*!
 * \inmodule BluezQt
 * \class BluezQt::Adapter
 * \inheaderfile BluezQt/Adapter
 *
 * \brief Bluetooth adapter.
 *
 * This class represents a Bluetooth adapter.
 */
class BLUEZQT_EXPORT Adapter : public QObject
{
    Q_OBJECT

    /*! \property BluezQt::Adapter::ubi */
    Q_PROPERTY(QString ubi READ ubi)
    /*! \property BluezQt::Adapter::address */
    Q_PROPERTY(QString address READ address)
    /*! \property BluezQt::Adapter::name */
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    /*! \property BluezQt::Adapter::systemName */
    Q_PROPERTY(QString systemName READ systemName NOTIFY systemNameChanged)
    /*! \property BluezQt::Adapter::adapterClass */
    Q_PROPERTY(quint32 adapterClass READ adapterClass NOTIFY adapterClassChanged)
    /*! \property BluezQt::Adapter::powered */
    Q_PROPERTY(bool powered READ isPowered WRITE setPowered NOTIFY poweredChanged)
    /*! \property BluezQt::Adapter::discoverable */
    Q_PROPERTY(bool discoverable READ isDiscoverable WRITE setDiscoverable NOTIFY discoverableChanged)
    /*! \property BluezQt::Adapter::discoverableTimeout */
    Q_PROPERTY(quint32 discoverableTimeout READ discoverableTimeout WRITE setDiscoverableTimeout NOTIFY discoverableTimeoutChanged)
    /*! \property BluezQt::Adapter::pairable */
    Q_PROPERTY(bool pairable READ isPairable WRITE setPairable NOTIFY pairableChanged)
    /*! \property BluezQt::Adapter::pairableTimeout */
    Q_PROPERTY(quint32 pairableTimeout READ pairableTimeout WRITE setPairableTimeout NOTIFY pairableTimeoutChanged)
    /*! \property BluezQt::Adapter::discovering */
    Q_PROPERTY(bool discovering READ isDiscovering NOTIFY discoveringChanged)
    /*! \property BluezQt::Adapter::uuids */
    Q_PROPERTY(QStringList uuids READ uuids NOTIFY uuidsChanged)
    /*! \property BluezQt::Adapter::modalias */
    Q_PROPERTY(QString modalias READ modalias NOTIFY modaliasChanged)
    /*! \property BluezQt::Adapter::leAdvertisingManager */
    Q_PROPERTY(LEAdvertisingManagerPtr leAdvertisingManager READ leAdvertisingManager NOTIFY leAdvertisingManagerChanged)
    /*! \property BluezQt::Adapter::media */
    Q_PROPERTY(MediaPtr media READ media NOTIFY mediaChanged)
    /*! \property BluezQt::Adapter::devices */
    Q_PROPERTY(QList<DevicePtr> devices READ devices)

public:
    ~Adapter() override;

    /*!
     * Returns a shared pointer from this.
     */
    AdapterPtr toSharedPtr() const;

    /*!
     * Returns an UBI of the adapter.
     *
     * Example UBI: "/org/bluez/hci0"
     */
    QString ubi() const;

    /*!
     * Returns an address of the adapter.
     *
     * Example address: "1C:E5:C3:BC:94:7E"
     */
    QString address() const;

    /*!
     * Returns the name of the adapter.
     */
    QString name() const;

    /*!
     * Sets the \a name of the adapter.
     *
     * Returns void pending call.
     */
    PendingCall *setName(const QString &name);

    /*!
     * Returns a system name (hostname) of the adapter.
     */
    QString systemName() const;

    /*!
     * Returns a class of the adapter.
     */
    quint32 adapterClass() const;

    /*!
     * Returns whether the adapter is powered on.
     */
    bool isPowered() const;

    /*!
     * Sets the \a powered state of the adapter.
     *
     * Returns void pending call.
     */
    PendingCall *setPowered(bool powered);

    /*!
     * Returns whether the adapter is discoverable by other devices.
     */
    bool isDiscoverable() const;

    /*!
     * Sets the \a discoverable state of the adapter.
     *
     * Returns void pending call.
     */
    PendingCall *setDiscoverable(bool discoverable);

    /*!
     * Returns the discoverable timeout in seconds of the adapter.
     *
     * The discoverable timeout defines how long the adapter stays in
     * discoverable state after calling setDiscoverable(true).
     *
     * Timeout 0 means infinitely.
     */
    quint32 discoverableTimeout() const;

    /*!
     * Sets the discoverable \a timeout of the adapter in seconds.
     *
     * Returns void pending call.
     */
    PendingCall *setDiscoverableTimeout(quint32 timeout);

    /*!
     * Returns whether the adapter is pairable with other devices.
     */
    bool isPairable() const;

    /*!
     * Sets the \a pairable state of the adapter.
     *
     * Returns void pending call.
     */
    PendingCall *setPairable(bool pairable);

    /*!
     * Returns the pairable timeout in seconds of the adapter.
     *
     * The pairable timeout defines how long the adapter stays in
     * pairable state after calling setPairable(true).
     *
     * Timeout 0 means infinitely.
     */
    quint32 pairableTimeout() const;

    /*!
     * Sets the pairable \a timeout of the adapter in seconds.
     *
     * Returns void pending call.
     */
    PendingCall *setPairableTimeout(quint32 timeout);

    /*!
     * Returns whether the adapter is discovering for other devices.
     */
    bool isDiscovering();

    /*!
     * Returns UUIDs of supported services by the adapter in uppercase.
     */
    QStringList uuids() const;

    /*!
     * Returns local device ID in modalias format.
     */
    QString modalias() const;

    /*!
     * Returns the GATT manager interface for the adapter.
     *
     * Returns null if the adapter has no GATT manager.
     */
    GattManagerPtr gattManager() const;

    /*!
     * Returns the LE advertising manager interface for the adapter.
     *
     * Returns null if the adapter has no Bluetooth LE.
     */
    LEAdvertisingManagerPtr leAdvertisingManager() const;

    /*!
     * Returns the media interface for the adapter.
     *
     * Returns null if the adapter has no media.
     */
    MediaPtr media() const;

    /*!
     * Returns list of devices known by the adapter.
     */
    QList<DevicePtr> devices() const;

    /*!
     * Returns a device with the specified \a address.
     *
     * The \a address follows a scheme similar to "40:79:6A:0C:39:75".
     *
     * Returns null if there is no device with the specified address.
     */
    DevicePtr deviceForAddress(const QString &address) const;

    /*!
     * Starts device discovery.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::NotReady
     * \li PendingCall::Failed
     * \endlist
     *
     * Returns void pending call.
     * \sa discoverableTimeout()
     */
    PendingCall *startDiscovery();

    /*!
     * Stops device discovery.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::NotReady
     * \li PendingCall::Failed
     * \li PendingCall::NotAuthorized
     * \endlist
     *
     * Returns void pending call.
     */
    PendingCall *stopDiscovery();

    /*!
     * Removes the specified \a device.
     *
     * It will also remove the pairing information.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::InvalidArguments
     * \li PendingCall::Failed
     * \endlist
     *
     * Returns void pending call.
     */
    PendingCall *removeDevice(DevicePtr device);

    /*!
     * Sets the discovery \a filter for the caller.
     *
     * When this method is called with no filter parameter, the filter is removed.
     *
     * For details and available filter options, see the
     * \l {https://git.kernel.org/pub/scm/bluetooth/bluez.git/tree/doc/adapter-api.txt}
     * {Bluez documentation for Adapter object}.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::InvalidArguments
     * \li PendingCall::Failed
     * \endlist
     *
     * Returns void pending call.
     */
    PendingCall *setDiscoveryFilter(const QVariantMap &filter);

    /*!
     * Gets the discovery filters for the caller.
     *
     * This returns the available filters that can be given to setDiscoveryFilter.
     *
     * For details and available filter options, see the
     * \l {https://git.kernel.org/pub/scm/bluetooth/bluez.git/tree/doc/adapter-api.txt}
     * {Bluez documentation for Adapter object}.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::Failed
     * \endlist
     *
     * Returns string list pending call.
     */
    PendingCall *getDiscoveryFilters();

Q_SIGNALS:
    /*!
     * Indicates that the adapter was removed.
     */
    void adapterRemoved(AdapterPtr adapter);

    /*!
     * Indicates that at least one of the adapter's properties have changed.
     */
    void adapterChanged(AdapterPtr adapter);

    /*!
     * Indicates that adapter's name has changed.
     */
    void nameChanged(const QString &name);

    /*!
     * Indicates that adapter's system name has changed.
     */
    void systemNameChanged(const QString &systemName);

    /*!
     * Indicates that adapter's class has changed.
     */
    void adapterClassChanged(quint32 adapterClass);

    /*!
     * Indicates that adapter's powered state has changed.
     */
    void poweredChanged(bool powered);

    /*!
     * Indicates that adapter's discoverable state has changed.
     */
    void discoverableChanged(bool discoverable);

    /*!
     * Indicates that adapter's discoverable timeout has changed.
     */
    void discoverableTimeoutChanged(quint32 timeout);

    /*!
     * Indicates that adapter's pairable state has changed.
     */
    void pairableChanged(bool pairable);

    /*!
     * Indicates that adapter's pairable timeout has changed.
     */
    void pairableTimeoutChanged(quint32 timeout);

    /*!
     * Indicates that adapter's discovering state has changed.
     */
    void discoveringChanged(bool discovering);

    /*!
     * Indicates that adapter's UUIDs has changed.
     */
    void uuidsChanged(const QStringList &uuids);

    /*!
     * Indicates that adapter's modalias has changed.
     */
    void modaliasChanged(const QString &modalias);

    /*!
     * Indicates that adapter's GATT manager has changed.
     */
    void gattManagerChanged(GattManagerPtr gattManager);

    /*!
     * Indicates that adapter's LE advertising manager has changed.
     */
    void leAdvertisingManagerChanged(LEAdvertisingManagerPtr leAdvertisingManager);

    /*!
     * Indicates that adapter's media has changed.
     */
    void mediaChanged(MediaPtr media);

    /*!
     * Indicates that a new device was added (e.g. found by discovery).
     */
    void deviceAdded(DevicePtr device);

    /*!
     * Indicates that a device was removed.
     */
    void deviceRemoved(DevicePtr device);

    /*!
     * Indicates that at least one of the device's properties has changed.
     */
    void deviceChanged(DevicePtr device);

private:
    BLUEZQT_NO_EXPORT explicit Adapter(const QString &path, const QVariantMap &properties);

    std::unique_ptr<class AdapterPrivate> const d;

    friend class AdapterPrivate;
    friend class ManagerPrivate;
    friend class InitAdaptersJobPrivate;
};

} // namespace BluezQt

#endif // BLUEZQT_ADAPTER_H
