#include "device_p.h"
#include "device.h"

using namespace QBluez;

DevicePrivate::DevicePrivate(const QString &path, Adapter *adapter, Device *parent)
    : QObject(parent)
    , q(parent)
    , m_adapter(adapter)
    , m_path(path)
{
}
