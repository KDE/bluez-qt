#ifndef QBLUEZ_ADAPTER_H
#define QBLUEZ_ADAPTER_H

#include <QObject>
#include <QList>
#include <QStringList>

#include <QBluez/LoadAdapterJob>

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
    void setAlias(const QString &alias);

    quint32 adapterClass() const;
    void setAdapterClass(quint32 adapterClass);

    bool isPowered() const;
    void setPowered(bool powered);

    bool isDiscoverable() const;
    void setDiscoverable(bool discoverable);

    quint32 discoverableTimeout() const;
    void setDiscoverableTimeout(quint32 timeout);

    bool isPairable() const;
    void setPairable(bool pairable);

    quint32 pairableTimeout() const;
    void setPairableTimeout(quint32 timeout);

    bool isDiscovering();
    void setDiscovering(bool discovering);

    QStringList UUIDs() const;

    QString modalias() const;

    QList<Device *> devices() const;

Q_SIGNALS:
    void deviceFound(Device *device);
    void deviceRemoved(Device *device);

private:
    Adapter(const QString &address, QObject *parent);

    AdapterPrivate *const d;

    friend class AdapterPrivate;
    friend class ManagerPrivate;
};

} // namespace QBluez

#endif // QBLUEZ_ADAPTER_H
