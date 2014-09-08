#ifndef QBLUEZ_DEVICE_H
#define QBLUEZ_DEVICE_H

#include <QObject>

#include "utils.h"
#include "qbluez_export.h"

namespace QBluez
{

class Adapter;
class PendingCall;

/**
 * Bluetooth device.
 *
 * This class represents a Bluetooth device.
 */
class QBLUEZ_EXPORT Device : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString ubi READ ubi)
    Q_PROPERTY(QString address READ address)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString friendlyName READ friendlyName NOTIFY friendlyNameChanged)
    Q_PROPERTY(QString alias READ alias WRITE setAlias NOTIFY aliasChanged)
    Q_PROPERTY(quint32 deviceClass READ deviceClass NOTIFY deviceClassChanged)
    Q_PROPERTY(DeviceType deviceType READ deviceType NOTIFY deviceTypeChanged)
    Q_PROPERTY(quint16 appearance READ appearance NOTIFY appearanceChanged)
    Q_PROPERTY(QString icon READ icon NOTIFY iconChanged)
    Q_PROPERTY(bool paired READ isPaired NOTIFY pairedChanged)
    Q_PROPERTY(bool trusted READ isTrusted WRITE setTrusted NOTIFY trustedChanged)
    Q_PROPERTY(bool blocked READ isBlocked WRITE setBlocked NOTIFY blockedChanged)
    Q_PROPERTY(bool legacyPairing READ hasLegacyPairing NOTIFY legacyPairingChanged)
    Q_PROPERTY(qint16 rssi READ rssi NOTIFY rssiChanged)
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)
    Q_PROPERTY(QStringList uuids READ uuids NOTIFY uuidsChanged)
    Q_PROPERTY(QString modalias READ modalias NOTIFY modaliasChanged)
    Q_PROPERTY(QBluez::Adapter* adapter READ adapter)

public:
    /**
     * Destroys a Device object.
     */
    ~Device();

    /**
     * Returns an UBI of the device.
     *
     * Example UBI: "/org/bluez/hci0/dev_40_79_6A_0C_39_75"
     *
     * @return UBI of device
     */
    QString ubi() const;

    /**
     * Returns an address of the device.
     *
     * Example address: "40:79:6A:0C:39:75"
     *
     * @return address of device
     */
    QString address() const;

    /**
     * Returns a name of the device.
     *
     * @return  name of device
     */
    QString name() const;

    /**
     * Returns a friendly name of the device.
     *
     * Friendly name is a string "alias (name)".
     * If the alias is empty or same as name, it returns just name.
     *
     * @return friendly name of device
     */
    QString friendlyName() const;

    /**
     * Returns an alias of the device.
     *
     * @return alias of device
     */
    QString alias() const;

    /**
     * Sets the alias of the device.
     *
     * @param alias alias for device
     * @return void pending call
     */
    PendingCall *setAlias(const QString &alias);

    /**
     * Returns a class of the device.
     *
     * @return class of device
     */
    quint32 deviceClass() const;

    /**
     * Returns a type of the device.
     *
     * @return type of device
     */
    DeviceType deviceType() const;

    /**
     * Returns an appearance of the device.
     *
     * @return appearance of device
     */
    quint16 appearance() const;

    /**
     * Returns an icon name of the device.
     *
     * In case the icon is empty, "preferences-system-bluetooth" is returned.
     *
     * @return icon name of device
     */
    QString icon() const;

    /**
     * Returns whether the device is paired.
     *
     * @return true if device is paired
     */
    bool isPaired() const;

    /**
     * Returns whether the device is trusted.
     *
     * @return true if device is trusted
     */
    bool isTrusted() const;

    /**
     * Sets the trusted state of the device.
     *
     * @param trusted trusted state
     * @return void pending call
     */
    PendingCall *setTrusted(bool trusted);

    /**
     * Returns whether the device is blocked.
     *
     * @return true if device is blocked
     */
    bool isBlocked() const;

    /**
     * Sets the blocked state of the device.
     *
     * @param blocked blocked state
     * @return void pending call
     */
    PendingCall *setBlocked(bool blocked);

    /**
     * Returns whether the device has legacy pairing.
     *
     * @return true if device has legacy pairing
     */
    bool hasLegacyPairing() const;

    /**
     * Returns Received Signal Strength Indicator of the device.
     *
     * @return RSSI of device
     */
    qint16 rssi() const;

    /**
     * Returns whether the device is connected.
     *
     * @return true if connected
     */
    bool isConnected() const;

    /**
     * Returns UUIDs of supported services by the device.
     *
     * UUIDs will always be returned in uppercase.
     *
     * @return UUIDs of supported services
     */
    QStringList uuids() const;

    /**
     * Returns remote device ID in modalias format.
     *
     * @return device modalias
     */
    QString modalias() const;

    /**
     * Returns an adapter that discovered this device.
     *
     * @return adapter of device
     */
    Adapter *adapter() const;

public Q_SLOTS:
    /**
     * Connects all auto-connectable profiles of the device.
     *
     * This method indicates success if at least one profile was connected.
     *
     * Possible errors: PendingCall::NotReady, PendingCall::Failed,
     *                  PendingCall::InProgress, PendingCall::AlreadyConnected
     *
     * @return void pending call
     */
    QBluez::PendingCall *connectDevice();

    /**
     * Disconnects all connected profiles of the device.
     *
     * This method can be used to cancel not-yet finished connectDevice() call.
     *
     * Possible errors: PendingCall::NotConnected
     *
     * @return void pending call
     */
    QBluez::PendingCall *disconnectDevice();

    /**
     * Connects a specific profile of the device.
     *
     * Possible errors: PendingCall::DoesNotExist, PendingCall::AlreadyConnected,
     *                  PendingCall::ConnectFailed
     *
     * @param uuid service UUID
     * @return void pending call
     */
    QBluez::PendingCall *connectProfile(const QString &uuid);

    /**
     * Disconnects a specific profile of the device.
     *
     * Possible errors: PendingCall::DoesNotExist, PendingCall::Failed,
     *                  PendingCall::NotConnected, PendingCall::NotSupported
     *
     * @param uuid service UUID
     * @return void pending call
     */
    QBluez::PendingCall *disconnectProfile(const QString &uuid);

    /**
     * Initiates a pairing of the device.
     *
     * Possible errors: PendingCall::InvalidArguments, PendingCall::Failed,
     *                  PendingCall::AlreadyExists, PendingCall::AuthenticationCanceled,
     *                  PendingCall::AuthenticationFailed, PendingCall::AuthenticationRejected,
     *                  PendingCall::AuthenticationTimeout, PendingCall::ConnectionAttemptFailed
     *
     * @return void pending call
     */
    QBluez::PendingCall *pair();

    /**
     * Cancels a pairing of the device.
     *
     * This method can be used to cancel pairing operation inititated with pair().
     *
     * Possible errors: PendingCall::DoesNotExist, PendingCall::Failed
     *
     * @return void pending call
     */
    QBluez::PendingCall *cancelPairing();

Q_SIGNALS:
    /**
     * Indicates that at least one of the device's property have changed.
     */
    void deviceChanged(QBluez::Device *device);

    /**
     * Indicates that device's name have changed.
     */
    void nameChanged(const QString &name);

    /**
     * Indicates that device's friendly name have changed.
     */
    void friendlyNameChanged(const QString &friendlyName);

    /**
     * Indicates that device's alias have changed.
     */
    void aliasChanged(const QString &alias);

    /**
     * Indicates that device's class have changed.
     */
    void deviceClassChanged(quint32 deviceClass);

    /**
     * Indicates that device's type have changed.
     */
    void deviceTypeChanged(DeviceType deviceType);

    /**
     * Indicates that device's appearance have changed.
     */
    void appearanceChanged(quint16 appearance);

    /**
     * Indicates that device's icon have changed.
     */
    void iconChanged(const QString &icon);

    /**
     * Indicates that device's paired state have changed.
     */
    void pairedChanged(bool paired);

    /**
     * Indicates that device's trusted state have changed.
     */
    void trustedChanged(bool trusted);

    /**
     * Indicates that device's blocked state have changed.
     */
    void blockedChanged(bool blocked);

    /**
     * Indicates that device's legacy pairing state have changed.
     */
    void legacyPairingChanged(bool legacyPairing);

    /**
     * Indicates that device's RSSI have changed.
     */
    void rssiChanged(qint16 rssi);

    /**
     * Indicates that device's connected state have changed.
     */
    void connectedChanged(bool connected);

    /**
     * Indicates that device's UUIDs have changed.
     */
    void uuidsChanged(const QStringList &uuids);

    /**
     * Indicates that device's modalias have changed.
     */
    void modaliasChanged(const QString &modalias);

private:
    explicit Device(const QString &address, const QVariantMap &properties, Adapter *adapter, QObject *parent);

    class DevicePrivate *const d;

    friend class DevicePrivate;
    friend class ManagerPrivate;
    friend class Adapter;
};

} // namespace QBluez

#endif // QBLUEZ_DEVICE_H
