#include "manager.h"
#include "manager_p.h"
#include "adapterinfo.h"

using namespace QBluez;

static Manager *instance = 0;

Manager::Manager()
    : QObject()
    , d(new ManagerPrivate(this))
{
    d->initialize();
}

Manager::~Manager()
{
    delete d;
}

// static
Manager *Manager::self()
{
    if (!instance) {
        instance = new Manager();
    }
    return instance;
}

// static
void Manager::release()
{
    delete instance;
    instance = 0;
}

QList<AdapterInfo *> Manager::adapters() const
{
    return d->m_adapters.values();
}

QList<DeviceInfo *> Manager::devices() const
{
    QList<DeviceInfo *> list;

    Q_FOREACH (AdapterInfo *adapter, d->m_adapters.values()) {
        list.append(adapter->devices());
    }

    return list;
}

bool Manager::isOperational() const
{
    return d->m_bluezRunning && d->m_initialized;
}
