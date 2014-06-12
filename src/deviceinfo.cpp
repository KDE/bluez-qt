#include "deviceinfo.h"

using namespace QBluez;

DeviceInfo::DeviceInfo(const QString &path, QObject *parent)
    : QObject(parent)
    , m_path(path)
    , m_device(0)
{
}

DeviceInfo::~DeviceInfo()
{
    //delete m_device;
}

QString DeviceInfo::path() const
{
    return m_path;
}
