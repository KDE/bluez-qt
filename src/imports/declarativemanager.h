#ifndef DECLARATIVEMANAGER_H
#define DECLARATIVEMANAGER_H

#include "manager.h"

#include <QQmlListProperty>

class DeclarativeManager : public QBluez::Manager
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<QBluez::Adapter> adapters READ declarativeAdapters NOTIFY adaptersChanged)
    Q_PROPERTY(QQmlListProperty<QBluez::Device> devices READ declarativeDevices NOTIFY devicesChanged)

public:
    DeclarativeManager(QObject *parent = 0);

    QQmlListProperty<QBluez::Adapter> declarativeAdapters();
    QQmlListProperty<QBluez::Device> declarativeDevices();

    static DeclarativeManager *self();

Q_SIGNALS:
    void initialized();
    void initializeError(const QString &errorText);

    void adaptersChanged(QQmlListProperty<QBluez::Adapter> adapters);
    void devicesChanged(QQmlListProperty<QBluez::Device> devices);

private:
    void initJobResult(QBluez::InitManagerJob *job);

    void slotAdapterAdded(QBluez::Adapter *adapter);
    void slotAdapterRemoved(QBluez::Adapter *adapter);
    void slotDeviceAdded(QBluez::Device *device);
    void slotDeviceRemoved(QBluez::Device *device);
};

#endif // DECLARATIVEMANAGER_H
