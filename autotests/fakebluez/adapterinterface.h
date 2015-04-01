#ifndef ADAPTERINTERFACE_H
#define ADAPTERINTERFACE_H

#include "object.h"

#include <QDBusAbstractAdaptor>

class QDBusObjectPath;

class AdapterObject : public QObject
{
public:
    explicit AdapterObject(const QDBusObjectPath &path, QObject *parent = Q_NULLPTR);
};

class AdapterInterface : public QDBusAbstractAdaptor, public Object
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.bluez.Adapter1")
    Q_PROPERTY(QString Address READ address)
    Q_PROPERTY(QString Name READ name)
    Q_PROPERTY(QString Alias READ alias WRITE setAlias)
    Q_PROPERTY(quint32 Class READ adapterClass)
    Q_PROPERTY(bool Powered READ powered WRITE setPowered)
    Q_PROPERTY(bool Discoverable READ discoverable WRITE setDiscoverable)
    Q_PROPERTY(bool Pairable READ pairable WRITE setPairable)
    Q_PROPERTY(quint32 PairableTimeout READ pairableTimeout WRITE setPairableTimeout)
    Q_PROPERTY(quint32 DiscoverableTimeout READ discoverableTimeout WRITE setDiscoverableTimeout)
    Q_PROPERTY(bool Discovering READ discovering)
    Q_PROPERTY(QStringList UUIDs READ uuids)
    Q_PROPERTY(QString Modalias READ modalias)

public:
    explicit AdapterInterface(const QDBusObjectPath &path, const QVariantMap &properties, QObject *parent = Q_NULLPTR);

    QString address() const;

    QString name() const;

    QString alias() const;
    void setAlias(const QString &alias);

    quint32 adapterClass() const;

    bool powered() const;
    void setPowered(bool powered);

    bool discoverable() const;
    void setDiscoverable(bool discoverable);

    bool pairable() const;
    void setPairable(bool pairable);

    quint32 pairableTimeout() const;
    void setPairableTimeout(quint32 timeout);

    quint32 discoverableTimeout() const;
    void setDiscoverableTimeout(quint32 timeout);

    bool discovering() const;

    QStringList uuids() const;

    QString modalias() const;

public Q_SLOTS:
    void StartDiscovery();
    void StopDiscovery();
    void RemoveDevice(const QDBusObjectPath &device);

private Q_SLOTS:
    void resetPairable();
    void resetDiscoverable();
};

#endif // ADAPTERINTERFACE_H
