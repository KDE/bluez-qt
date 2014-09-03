#include "declarativemanager.h"
#include "initmanagerjob.h"
#include "adapter.h"
#include "device.h"

static int adaptersCountFunction(QQmlListProperty<QBluez::Adapter> *property)
{
    Q_ASSERT(qobject_cast<DeclarativeManager*>(property->object));
    DeclarativeManager *manager = static_cast<DeclarativeManager*>(property->object);

    return manager->adapters().count();
}

static QBluez::Adapter *adaptersAtFunction(QQmlListProperty<QBluez::Adapter> *property, int index)
{
    Q_ASSERT(qobject_cast<DeclarativeManager*>(property->object));
    DeclarativeManager *manager = static_cast<DeclarativeManager*>(property->object);

    return manager->adapters().at(index);
}

static int devicesCountFunction(QQmlListProperty<QBluez::Device> *property)
{
    Q_ASSERT(qobject_cast<DeclarativeManager*>(property->object));
    DeclarativeManager *manager = static_cast<DeclarativeManager*>(property->object);

    return manager->devices().count();
}

static QBluez::Device *devicesAtFunction(QQmlListProperty<QBluez::Device> *property, int index)
{
    Q_ASSERT(qobject_cast<DeclarativeManager*>(property->object));
    DeclarativeManager *manager = static_cast<DeclarativeManager*>(property->object);

    return manager->devices().at(index);
}

DeclarativeManager::DeclarativeManager(QObject *parent)
    : QBluez::Manager(parent)
{
    QBluez::InitManagerJob *job = init();
    job->start();
    connect(job, &QBluez::InitManagerJob::result, this, &DeclarativeManager::initJobResult);

    connect(this, &QBluez::Manager::adapterAdded, this, &DeclarativeManager::slotAdapterAdded);
    connect(this, &QBluez::Manager::adapterRemoved, this, &DeclarativeManager::slotAdapterRemoved);
}

QQmlListProperty<QBluez::Adapter> DeclarativeManager::declarativeAdapters()
{
    return QQmlListProperty<QBluez::Adapter>(this, 0, adaptersCountFunction, adaptersAtFunction);
}

QQmlListProperty<QBluez::Device> DeclarativeManager::declarativeDevices()
{
    return QQmlListProperty<QBluez::Device>(this, 0, devicesCountFunction, devicesAtFunction);
}

void DeclarativeManager::initJobResult(QBluez::InitManagerJob *job)
{
    if (job->error()) {
        Q_EMIT initializeError(job->errorText());
        return;
    }

    Q_EMIT initialized();
}

void DeclarativeManager::slotAdapterAdded(QBluez::Adapter *adapter)
{
    Q_EMIT adaptersChanged(declarativeAdapters());

    connect(adapter, &QBluez::Adapter::deviceFound, this, &DeclarativeManager::slotDeviceAdded);
    connect(adapter, &QBluez::Adapter::deviceRemoved, this, &DeclarativeManager::slotDeviceRemoved);
}

void DeclarativeManager::slotAdapterRemoved(QBluez::Adapter *adapter)
{
    Q_EMIT adaptersChanged(declarativeAdapters());

    disconnect(adapter, &QBluez::Adapter::deviceFound, this, &DeclarativeManager::slotDeviceAdded);
    disconnect(adapter, &QBluez::Adapter::deviceRemoved, this, &DeclarativeManager::slotDeviceRemoved);
}

void DeclarativeManager::slotDeviceAdded(QBluez::Device *device)
{
    Q_UNUSED(device)
    Q_EMIT devicesChanged(declarativeDevices());
}

void DeclarativeManager::slotDeviceRemoved(QBluez::Device *device)
{
    Q_UNUSED(device)
    Q_EMIT devicesChanged(declarativeDevices());
}
