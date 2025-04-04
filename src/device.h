/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * SPDX-FileCopyrightText: 2014-2015 David Rosca <nowrep@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef BLUEZQT_DEVICE_H
#define BLUEZQT_DEVICE_H

#include <QObject>

#include "bluezqt_export.h"
#include "types.h"

#include <memory>

namespace BluezQt
{
class Adapter;
class PendingCall;

/*!
 * \inmodule BluezQt
 * \class BluezQt::Device
 * \inheaderfile BluezQt/Device
 *
 * \brief Bluetooth device.
 *
 * This class represents a Bluetooth device.
 */
class BLUEZQT_EXPORT Device : public QObject
{
    Q_OBJECT

    /*! \property BluezQt::Device::ubi */
    Q_PROPERTY(QString ubi READ ubi)
    /*! \property BluezQt::Device::address */
    Q_PROPERTY(QString address READ address NOTIFY addressChanged)
    /*! \property BluezQt::Device::name */
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    /*! \property BluezQt::Device::friendlyName */
    Q_PROPERTY(QString friendlyName READ friendlyName NOTIFY friendlyNameChanged)
    /*! \property BluezQt::Device::remoteName */
    Q_PROPERTY(QString remoteName READ remoteName NOTIFY remoteNameChanged)
    /*! \property BluezQt::Device::deviceClass */
    Q_PROPERTY(quint32 deviceClass READ deviceClass NOTIFY deviceClassChanged)
    /*! \property BluezQt::Device::type */
    Q_PROPERTY(Type type READ type NOTIFY typeChanged)
    /*! \property BluezQt::Device::appearance */
    Q_PROPERTY(quint16 appearance READ appearance NOTIFY appearanceChanged)
    /*! \property BluezQt::Device::icon */
    Q_PROPERTY(QString icon READ icon NOTIFY iconChanged)
    /*! \property BluezQt::Device::paired */
    Q_PROPERTY(bool paired READ isPaired NOTIFY pairedChanged)
    /*! \property BluezQt::Device::trusted */
    Q_PROPERTY(bool trusted READ isTrusted WRITE setTrusted NOTIFY trustedChanged)
    /*! \property BluezQt::Device::blocked */
    Q_PROPERTY(bool blocked READ isBlocked WRITE setBlocked NOTIFY blockedChanged)
    /*! \property BluezQt::Device::legacyPairing */
    Q_PROPERTY(bool legacyPairing READ hasLegacyPairing NOTIFY legacyPairingChanged)
    /*! \property BluezQt::Device::rssi */
    Q_PROPERTY(qint16 rssi READ rssi NOTIFY rssiChanged)
    /*! \property BluezQt::Device::manufacturerData */
    Q_PROPERTY(ManData manufacturerData READ manufacturerData NOTIFY manufacturerDataChanged)
    /*! \property BluezQt::Device::servicesResolved */
    Q_PROPERTY(bool servicesResolved READ isServicesResolved NOTIFY servicesResolvedChanged)
    /*! \property BluezQt::Device::connected */
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)
    /*! \property BluezQt::Device::uuids */
    Q_PROPERTY(QStringList uuids READ uuids NOTIFY uuidsChanged)
    /*! \property BluezQt::Device::modalias */
    Q_PROPERTY(QString modalias READ modalias NOTIFY modaliasChanged)
    /*! \property BluezQt::Device::battery */
    Q_PROPERTY(BatteryPtr battery READ battery NOTIFY batteryChanged)
    /*! \property BluezQt::Device::input */
    Q_PROPERTY(InputPtr input READ input NOTIFY inputChanged)
    /*! \property BluezQt::Device::mediaPlayer */
    Q_PROPERTY(MediaPlayerPtr mediaPlayer READ mediaPlayer NOTIFY mediaPlayerChanged)
    /*! \property BluezQt::Device::mediaTransport */
    Q_PROPERTY(MediaTransportPtr mediaTransport READ mediaTransport NOTIFY mediaTransportChanged)
    /*! \property BluezQt::Device::adapter */
    Q_PROPERTY(AdapterPtr adapter READ adapter)
    /*! \property BluezQt::Device::gattServices */
    Q_PROPERTY(QList<GattServiceRemotePtr> gattServices READ gattServices NOTIFY gattServicesChanged)

public:
    /*!
     * Device types.
     * \value Phone
     *        The device is a phone.
     * \value Modem
     *        The device is a modem.
     * \value Computer
     *        The device is a computer.
     * \value Network
     *        The device is a computer.
     * \value Headset
     *        The device is a headset.
     * \value Headphones
     *        The device is a headphone.
     * \value AudioVideo
     *        The device is an uncategorized audio/video device.
     * \value Keyboard
     *        The device is a keyboard.
     * \value Mouse
     *        The device is a mouse.
     * \value Joypad
     *        The device is a joypad.
     * \value Tablet
     *        The device is a graphics tablet (input device).
     * \value Peripheral
     *        The device is an uncategorized peripheral device.
     * \value Camera
     *        The device is a camera.
     * \value Printer
     *        The device is a printer.
     * \value Imaging
     *        The device is an uncategorized imaging device.
     * \value Wearable
     *        The device is a wearable device.
     * \value Toy
     *        The device is a toy.
     * \value Health
     *        The device is a health device.
     * \value Uncategorized
     *        The device is not of any of the known types.
     */
    enum Type {
        Phone,
        Modem,
        Computer,
        Network,
        Headset,
        Headphones,
        AudioVideo,
        Keyboard,
        Mouse,
        Joypad,
        Tablet,
        Peripheral,
        Camera,
        Printer,
        Imaging,
        Wearable,
        Toy,
        Health,
        Uncategorized,
    };
    Q_ENUM(Type)

    ~Device() override;

    /*!
     * Returns a shared pointer from this.
     */
    DevicePtr toSharedPtr() const;

    /*!
     * Returns the UBI of the device.
     *
     * Example UBI: "/org/bluez/hci0/dev_40_79_6A_0C_39_75"
     */
    QString ubi() const;

    /*!
     * Returns the address of the device.
     *
     * Example address: "40:79:6A:0C:39:75"
     */
    QString address() const;

    /*!
     * Returns the name of the device.
     *
     * If the name of the device wasn't previously changed,
     * remoteName is returned.
     */
    QString name() const;

    /*!
     * Sets the \a name of the device.
     *
     * Returns a void pending call.
     */
    PendingCall *setName(const QString &name);

    /*!
     * Returns the friendly name of the device.
     *
     * Friendly name is a string "name (remoteName)".
     * If the remoteName is same as name, it returns just name.
     */
    QString friendlyName() const;

    /*!
     * Returns the remote name of the device.
     */
    QString remoteName() const;

    /*!
     * Returns the class of the device.
     *
     * In case of Bluetooth Low Energy only devices,
     * device class is invalid (0).
     *
     * \sa type()
     */
    quint32 deviceClass() const;

    /*!
     * Returns the type of the device.
     *
     * Type of device is deduced from its class (for Bluetooth Classic devices)
     * or its appearance (for Bluetooth Low Energy devices).
     *
     * \sa deviceClass()
     * \sa appearance()
     */
    Device::Type type() const;

    /*!
     * Returns the appearance of the device.
     */
    quint16 appearance() const;

    /*!
     * Returns the icon name of the device.
     *
     * In case the icon is empty, "preferences-system-bluetooth" is returned.
     */
    QString icon() const;

    /*!
     * Returns whether the device is paired.
     */
    bool isPaired() const;

    /*!
     * Returns whether the device is trusted.
     */
    bool isTrusted() const;

    /*!
     * Sets the \a trusted state of the device.
     *
     * Returns void pending call.
     */
    PendingCall *setTrusted(bool trusted);

    /*!
     * Returns whether the device is blocked.
     */
    bool isBlocked() const;

    /*!
     * Sets the \a blocked state of the device.
     *
     * Returns void pending call.
     */
    PendingCall *setBlocked(bool blocked);

    /*!
     * Returns whether the device has legacy pairing.
     */
    bool hasLegacyPairing() const;

    /*!
     * Returns the Received Signal Strength Indicator of the device.
     *
     * The bigger value indicates better signal strength.
     *
     * \note RSSI is only updated during discovery.
     */
    qint16 rssi() const;

    /*!
     * Returns manufacturer specific advertisement data.
     *
     * \note Keys are 16 bits Manufacturer ID followed by
     * its byte array value.
     */
    ManData manufacturerData() const;

    /*!
     * Returns whether or not service discovery has been resolved.
     */
    bool isServicesResolved() const;

    /*!
     * Returns whether the device is connected.
     */
    bool isConnected() const;

    /*!
     * Returns UUIDs of services supported by the device.
     *
     * UUIDs will always be returned in uppercase.
     */
    QStringList uuids() const;

    /*!
     * Returns remote device ID in modalias format.
     */
    QString modalias() const;

    /*!
     * Returns the service advertisement data.
     *
     * Returns a hash with keys being the UUIDs in and values being the raw service data value.
     * \since 5.72
     */
    QHash<QString, QByteArray> serviceData() const;

    /*!
     * Returns the battery interface for the device.
     *
     * Returns null if device has no battery.
     * \since 5.66
     */
    BatteryPtr battery() const;

    /*!
     * Returns the input interface for the device.
     *
     * Only input devices will have valid input interface.
     *
     * Returns null if device has no input.
     */
    InputPtr input() const;

    /*!
     * Returns the media player interface for the device.
     *
     * Only devices with connected appropriate profile will
     * have valid media player interface.
     *
     * Returns null if device has no media player.
     */
    MediaPlayerPtr mediaPlayer() const;

    /*!
     * Returns the media transport interface for the device.
     *
     * Only devices with connected appropriate profile will
     * have valid media transport interface.
     *
     * Returns null if device has no media transport.
     */
    MediaTransportPtr mediaTransport() const;

    /*!
     * Returns the adapter that discovered this device.
     */
    AdapterPtr adapter() const;

    /*!
     * Returns a list of services known by the device.
     */
    QList<GattServiceRemotePtr> gattServices() const;
    /*!
     * Returns a string of the device \a type.
     */
    static QString typeToString(Device::Type type);

    /*!
     * Returns a string for the device type \a typeString.
     */
    static Device::Type stringToType(const QString &typeString);

public Q_SLOTS:
    /*!
     * Connects all auto-connectable profiles of the device.
     *
     * This method indicates success if at least one profile was connected.
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
    PendingCall *connectToDevice();

    /*!
     * Disconnects all connected profiles of the device.
     *
     * This method can be used to cancel not-yet finished connectDevice() call.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::NotConnected
     * \endlist
     *
     * Returns void pending call.
     */
    PendingCall *disconnectFromDevice();

    /*!
     * Connects a specific profile of the device with the given service \a uuid.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::DoesNotExist
     * \li PendingCall::AlreadyConnected
     * \li PendingCall::ConnectFailed
     * \endlist
     *
     * Returns void pending call.
     */
    PendingCall *connectProfile(const QString &uuid);

    /*!
     * Disconnects a specific profile of the device with the given service \a uuid.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::DoesNotExist
     * \li PendingCall::Failed
     * \li PendingCall::NotConnected
     * \li PendingCall::NotSupported
     * \endlist
     *
     * Returns void pending call.
     */
    PendingCall *disconnectProfile(const QString &uuid);

    /*!
     * Initiates a pairing with the device.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::InvalidArguments
     * \li PendingCall::Failed
     * \li PendingCall::AlreadyExists
     * \li PendingCall::AuthenticationCanceled
     * \li PendingCall::AuthenticationFailed
     * \li PendingCall::AuthenticationRejected
     * \li PendingCall::AuthenticationTimeout
     * \li PendingCall::ConnectionAttemptFailed
     * \endlist
     *
     * Returns void pending call.
     */
    PendingCall *pair();

    /*!
     * Cancels a pairing with the device.
     *
     * This method can be used to cancel pairing operation initiated with pair().
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::DoesNotExist
     * \li PendingCall::Failed
     * \endlist
     *
     * Returns void pending call.
     */
    PendingCall *cancelPairing();

Q_SIGNALS:
    /*!
     * Indicates that the device was removed.
     */
    void deviceRemoved(DevicePtr device);

    /*!
     * Indicates that at least one of the device's properties has changed.
     */
    void deviceChanged(DevicePtr device);

    /*!
     * Indicates that a new service was added (eg. found by connection).
     */
    void gattServiceAdded(GattServiceRemotePtr service);

    /*!
     * Indicates that the device GATT services list has changed.
     */
    void gattServicesChanged(QList<GattServiceRemotePtr> services);

    /*!
     * Indicates that a service was removed.
     */
    void gattServiceRemoved(GattServiceRemotePtr service);

    /*!
     * Indicates that at least one of the device's services has changed.
     */
    void gattServiceChanged(GattServiceRemotePtr service);

    /*!
     * Indicates that the device's name has changed.
     */
    void nameChanged(const QString &name);

    /*!
     * Indicates that the device's address has changed.
     */
    void addressChanged(const QString &address);

    /*!
     * Indicates that the device's friendly name has changed.
     */
    void friendlyNameChanged(const QString &friendlyName);

    /*!
     * Indicates that the device's remote name has changed.
     */
    void remoteNameChanged(const QString &remoteName);

    /*!
     * Indicates that the device's class has changed.
     */
    void deviceClassChanged(quint32 deviceClass);

    /*!
     * Indicates that the device's type has changed.
     */
    void typeChanged(Type type);

    /*!
     * Indicates that the device's appearance has changed.
     */
    void appearanceChanged(quint16 appearance);

    /*!
     * Indicates that the device's icon has changed.
     */
    void iconChanged(const QString &icon);

    /*!
     * Indicates that the device's paired state has changed.
     */
    void pairedChanged(bool paired);

    /*!
     * Indicates that the device's trusted state has changed.
     */
    void trustedChanged(bool trusted);

    /*!
     * Indicates that the device's blocked state has changed.
     */
    void blockedChanged(bool blocked);

    /*!
     * Indicates that the device's legacy pairing state has changed.
     */
    void legacyPairingChanged(bool legacyPairing);

    /*!
     * Indicates that the device's RSSI has changed.
     */
    void rssiChanged(qint16 rssi);

    /*!
     * Indicates that the device's manufacturer data has changed.
     */
    void manufacturerDataChanged(ManData man);

    /*!
     * Indicates that the device's servicesResolved state has changed.
     */
    void servicesResolvedChanged(bool servicesResolved);

    /*!
     * Indicates that the device's connected state has changed.
     */
    void connectedChanged(bool connected);

    /*!
     * Indicates that the device's UUIDs has changed.
     */
    void uuidsChanged(const QStringList &uuids);

    /*!
     * Indicates that the device's modalias has changed.
     */
    void modaliasChanged(const QString &modalias);

    /*!
     * Indicates that the device's service data has changed.
     * \since 5.72
     */
    void serviceDataChanged(const QHash<QString, QByteArray> &serviceData);

    /*!
     * Indicates that the device's battery has changed.
     */
    void batteryChanged(BatteryPtr battery);

    /*!
     * Indicates that the device's input has changed.
     */
    void inputChanged(InputPtr input);

    /*!
     * Indicates that the device's media player has changed.
     */
    void mediaPlayerChanged(MediaPlayerPtr mediaPlayer);

    /*!
     * Indicates that the device's media transport has changed.
     */
    void mediaTransportChanged(MediaTransportPtr mediaTransport);

private:
    BLUEZQT_NO_EXPORT explicit Device(const QString &path, const QVariantMap &properties, AdapterPtr adapter);

    std::unique_ptr<class DevicePrivate> const d;

    friend class DevicePrivate;
    friend class ManagerPrivate;
    friend class Adapter;
};

} // namespace BluezQt

Q_DECLARE_METATYPE(BluezQt::ManData)

#endif // BLUEZQT_DEVICE_H
