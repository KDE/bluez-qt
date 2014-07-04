#ifndef QBLUEZ_DEVICE_H
#define QBLUEZ_DEVICE_H

#include <QObject>

#include "loaddevicejob.h"
#include "setpropertyjob.h"
#include "qbluez_export.h"

namespace QBluez
{

class Adapter;
class LoadDeviceJob;
class DevicePrivate;

class QBLUEZ_EXPORT Device : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString path READ path)
    Q_PROPERTY(QString address READ address)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString friendlyName READ friendlyName NOTIFY friendlyNameChanged)
    Q_PROPERTY(QString alias READ alias WRITE setAlias NOTIFY aliasChanged)
    Q_PROPERTY(quint32 deviceClass READ deviceClass NOTIFY deviceClassChanged)
    Q_PROPERTY(quint16 appearance READ appearance NOTIFY appearanceChanged)
    Q_PROPERTY(QString icon READ icon NOTIFY iconChanged)
    Q_PROPERTY(bool isPaired READ isPaired NOTIFY pairedChanged)
    Q_PROPERTY(bool isTrusted READ isTrusted WRITE setTrusted NOTIFY trustedChanged)
    Q_PROPERTY(bool isBlocked READ isBlocked WRITE setBlocked NOTIFY blockedChanged)
    Q_PROPERTY(bool legacyPairing READ hasLegacyPairing NOTIFY legacyPairingChanged)
    Q_PROPERTY(qint16 rssi READ rssi NOTIFY rssiChanged)
    Q_PROPERTY(bool isConnected READ isConnected NOTIFY connectedChanged)
    Q_PROPERTY(QStringList uuids READ uuids NOTIFY uuidsChanged)
    Q_PROPERTY(QString modalias READ modalias NOTIFY modaliasChanged)
    Q_PROPERTY(Adapter* adapter READ adapter)

public:
    virtual ~Device();

    bool isLoaded() const;
    LoadDeviceJob *load();

    QString path() const;

    QString address() const;

    QString name() const;
    QString friendlyName() const;

    QString alias() const;
    SetPropertyJob *setAlias(const QString &alias);

    quint32 deviceClass() const;

    quint16 appearance() const;

    QString icon() const;

    bool isPaired() const;

    bool isTrusted() const;
    SetPropertyJob *setTrusted(bool isTrusted);

    bool isBlocked() const;
    SetPropertyJob *setBlocked(bool isBlocked);

    bool hasLegacyPairing() const;

    qint16 rssi() const;

    bool isConnected() const;

    QStringList uuids() const;

    QString modalias() const;

    Adapter *adapter() const;

    void pair();

    void connect();
    void disconnect();

Q_SIGNALS:
    void nameChanged(const QString &name);
    void friendlyNameChanged(const QString &friendlyName);
    void aliasChanged(const QString &alias);
    void deviceClassChanged(quint32 deviceClass);
    void appearanceChanged(quint16 appearance);
    void iconChanged(const QString &icon);
    void pairedChanged(bool isPaired);
    void trustedChanged(bool isTrusted);
    void blockedChanged(bool isBlocked);
    void legacyPairingChanged(bool legacyPairing);
    void rssiChanged(qint16 rssi);
    void connectedChanged(bool isConnected);
    void uuidsChanged(const QStringList &uuids);
    void modaliasChanged(const QString &modalias);

private:
    Device(const QString &address, Adapter *adapter, QObject *parent);

    DevicePrivate *const d;

    friend class DevicePrivate;
    friend class ManagerPrivate;
    friend class Adapter;
};

} // namespace QBluez

#endif // QBLUEZ_DEVICE_H
