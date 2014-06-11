#include "manager.h"
#include "manager_p.h"

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

bool Manager::isBluetoothOperational() const
{
    return false;
}
