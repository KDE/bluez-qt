#ifndef QBLUEZ_ADAPTER_H
#define QBLUEZ_ADAPTER_H

#include <QObject>
#include <QList>
#include <QStringList>

#include "pendingcall.h"
#include "qbluez_export.h"

namespace QBluez
{

class Device;

class QBLUEZ_EXPORT Adapter : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString path READ path)
    Q_PROPERTY(QString address READ address)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString alias READ alias WRITE setAlias NOTIFY aliasChanged)
    Q_PROPERTY(quint32 adapterClass READ adapterClass NOTIFY adapterClassChanged)
    Q_PROPERTY(bool powered READ isPowered WRITE setPowered NOTIFY poweredChanged)
    Q_PROPERTY(bool discoverable READ isDiscoverable WRITE setDiscoverable NOTIFY discoverableChanged)
    Q_PROPERTY(quint32 discoverableTimeout READ discoverableTimeout WRITE setDiscoverableTimeout NOTIFY discoverableTimeoutChanged)
    Q_PROPERTY(bool pairable READ isPairable WRITE setPairable NOTIFY pairableChanged)
    Q_PROPERTY(quint32 pairableTimeout READ pairableTimeout WRITE setPairableTimeout NOTIFY pairableTimeoutChanged)
    Q_PROPERTY(bool discovering READ isDiscovering NOTIFY discoveringChanged)
    Q_PROPERTY(QStringList uuids READ uuids NOTIFY uuidsChanged)
    Q_PROPERTY(QString modalias READ modalias NOTIFY modaliasChanged)
    Q_PROPERTY(QList<Device *> devices READ devices)

public:
    virtual ~Adapter();

    QString path() const;

    QString address() const;

    QString name() const;

    QString alias() const;
    PendingCall *setAlias(const QString &alias);

    quint32 adapterClass() const;

    bool isPowered() const;
    PendingCall *setPowered(bool powered);

    bool isDiscoverable() const;
    PendingCall *setDiscoverable(bool discoverable);

    quint32 discoverableTimeout() const;
    PendingCall *setDiscoverableTimeout(quint32 timeout);

    bool isPairable() const;
    PendingCall *setPairable(bool pairable);

    quint32 pairableTimeout() const;
    PendingCall *setPairableTimeout(quint32 timeout);

    bool isDiscovering();

    QStringList uuids() const;

    QString modalias() const;

    QList<Device *> devices() const;

    // Possible errors: NotReady, Failed
    PendingCall *startDiscovery();

    // Possible errors: NotReady, Failed, NotAuthorized
    PendingCall *stopDiscovery();

    // Possible errors: InvalidArguments, Failed
    PendingCall *removeDevice(Device *device);

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
    void uuidsChanged(const QStringList &uuids);
    void modaliasChanged(const QString &modalias);

    void deviceFound(Device *device);
    void deviceRemoved(Device *device);
    void deviceChanged(Device *device);

private:
    explicit Adapter(const QString &address, QObject *parent);

    class AdapterPrivate *const d;

    friend class AdapterPrivate;
    friend class ManagerPrivate;
    friend class LoadAdaptersJobPrivate;
};

} // namespace QBluez

#endif // QBLUEZ_ADAPTER_H
