#include "declarativedevicesmodel.h"
#include "declarativemanager.h"

DeclarativeDevicesModel::DeclarativeDevicesModel(QObject *parent)
    : QBluez::DevicesModel(0, parent)
    , m_manager(0)
{
}

DeclarativeManager *DeclarativeDevicesModel::manager() const
{
    return m_manager;
}

void DeclarativeDevicesModel::setManager(DeclarativeManager *manager)
{
    m_manager = manager;
    QBluez::DevicesModel::setManager(m_manager);
}
