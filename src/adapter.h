#ifndef QBLUEZ_ADAPTER_H
#define QBLUEZ_ADAPTER_H

#include <QObject>
#include <QList>
#include <QStringList>

#include <QBluez/LoadAdapterJob>
#include <QBluez/SetPropertyJob>

#include "qbluez_export.h"

namespace QBluez
{

class Device;
class AdapterPrivate;

class QBLUEZ_EXPORT Adapter : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString address READ address)
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString alias READ alias WRITE setAlias)
    Q_PROPERTY(quint32 adapterClass READ adapterClass)
    Q_PROPERTY(bool powered READ isPowered WRITE setPowered)
    Q_PROPERTY(bool discoverable READ isDiscoverable WRITE setDiscoverable)
    Q_PROPERTY(quint32 discoverableTimeout READ discoverableTimeout WRITE setDiscoverableTimeout)
    Q_PROPERTY(bool pairable READ isPairable WRITE setPairable)
    Q_PROPERTY(quint32 pairableTimeout READ pairableTimeout WRITE setPairableTimeout)
    Q_PROPERTY(bool discovering READ isDiscovering)
    Q_PROPERTY(QStringList UUIDs READ UUIDs)
    Q_PROPERTY(QString modalias READ modalias)
    Q_PROPERTY(QList<Device *> devices READ devices)

public:
    virtual ~Adapter();

    bool isLoaded() const;
    LoadAdapterJob *load() const;

    QString address() const;

    QString name() const;

    QString alias() const;
    SetPropertyJob *setAlias(const QString &alias);

    quint32 adapterClass() const;
    SetPropertyJob *setAdapterClass(quint32 adapterClass);

    bool isPowered() const;
    SetPropertyJob *setPowered(bool powered);

    bool isDiscoverable() const;
    SetPropertyJob *setDiscoverable(bool discoverable);

    quint32 discoverableTimeout() const;
    SetPropertyJob *setDiscoverableTimeout(quint32 timeout);

    bool isPairable() const;
    SetPropertyJob *setPairable(bool pairable);

    quint32 pairableTimeout() const;
    SetPropertyJob *setPairableTimeout(quint32 timeout);

    bool isDiscovering();

    QStringList UUIDs() const;

    QString modalias() const;

    QList<Device *> devices() const;

    void removeDevice(Device *device);

    void startDiscovery();
    void stopDiscovery();

Q_SIGNALS:
    void nameChanged(const QString &name);
    void aliasChanged(const QString &name);
    void adapterClassChanged(quint32 adapterClass);
    void poweredChanged(bool powered);
    void discoverableChanged(bool discoverable);
    void discoverableTimeoutChanged(quint32 timeout);
    void pairableChanged(bool pairable);
    void pairableTimeoutChanged(quint32 timeout);
    void discoveringChanged(bool discovering);
    void UUIDsChanged(const QStringList &UUIDs);
    void modaliasChanged(const QString &modalias);

    void deviceFound(Device *device);
    void deviceRemoved(Device *device);
    void deviceChanged(Device *device);

private:
    Adapter(const QString &address, QObject *parent);

    AdapterPrivate *const d;

    friend class AdapterPrivate;
    friend class ManagerPrivate;
};

} // namespace QBluez

#endif // QBLUEZ_ADAPTER_H
