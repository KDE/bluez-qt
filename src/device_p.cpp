#include "device_p.h"
#include "device.h"

using namespace QBluez;

DevicePrivate::DevicePrivate(Device *parent)
    : QObject(parent)
    , q(parent)
    , m_adapter(0)
{
}
