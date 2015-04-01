#ifndef DEVICEINTERFACE_H
#define DEVICEINTERFACE_H

#include "object.h"

#include <QDBusAbstractAdaptor>

class QDBusObjectPath;

class DeviceObject : public QObject
{
public:
    explicit DeviceObject(const QDBusObjectPath &path, QObject *parent = Q_NULLPTR);
};

class DeviceInterface : public QDBusAbstractAdaptor, public Object
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.bluez.Device1")
    Q_PROPERTY(QString Address READ address)
    Q_PROPERTY(QString Name READ name)
    Q_PROPERTY(QString Alias READ alias WRITE setAlias)
    Q_PROPERTY(QString Icon READ icon)
    Q_PROPERTY(quint32 Class READ deviceClass)
    Q_PROPERTY(quint16 Appearance READ appearance)
    Q_PROPERTY(bool Paired READ paired)
    Q_PROPERTY(bool Connected READ connected)
    Q_PROPERTY(bool Trusted READ trusted WRITE setTrusted)
    Q_PROPERTY(bool Blocked READ blocked WRITE setBlocked)
    Q_PROPERTY(QDBusObjectPath Adapter READ adapter)
    Q_PROPERTY(bool LegacyPairing READ legacyPairing)
    Q_PROPERTY(qint16 RSSI READ rssi)
    Q_PROPERTY(QStringList UUIDs READ uuids)
    Q_PROPERTY(QString Modalias READ modalias)

public:
    explicit DeviceInterface(const QDBusObjectPath &path, const QVariantMap &properties, QObject *parent = Q_NULLPTR);

    QString address() const;

    QString name() const;

    QString alias() const;
    void setAlias(const QString &alias);

    QString icon() const;

    quint32 deviceClass() const;

    quint16 appearance() const;

    bool paired() const;

    bool connected() const;

    bool trusted() const;
    void setTrusted(bool trusted);

    bool blocked() const;
    void setBlocked(bool blocked);

    QDBusObjectPath adapter() const;

    bool legacyPairing() const;

    qint16 rssi() const;

    QStringList uuids() const;

    QString modalias() const;

public Q_SLOTS:
    void Connect();
    void Disconnect();
    void ConnectProfile(const QString &uuid);
    void DisconnectProfile(const QString &uuid);
    void Pair();
    void CancelPairing();
};

#endif // DEVICEINTERFACE_H
