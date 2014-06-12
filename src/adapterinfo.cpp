#include "adapterinfo.h"

using namespace QBluez;

AdapterInfo::AdapterInfo(const QString &path, QObject *parent)
    : QObject(parent)
    , m_path(path)
    , m_adapter(0)
{
}

AdapterInfo::~AdapterInfo()
{
    //delete m_adapter;
}

QString AdapterInfo::path() const
{
    return m_path;
}

QList<DeviceInfo *> AdapterInfo::devices() const
{
    return m_devices;
}

void AdapterInfo::addDevice(DeviceInfo *device)
{
    m_devices.append(device);
    emit deviceAdded(device);
}

void AdapterInfo::removeDevice(DeviceInfo *device)
{
    m_devices.removeOne(device);
    emit deviceRemoved(device);
}
