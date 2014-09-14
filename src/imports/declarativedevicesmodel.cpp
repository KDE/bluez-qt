#include "declarativedevicesmodel.h"
#include "declarativemanager.h"

DeclarativeDevicesModel::DeclarativeDevicesModel(QObject *parent)
    : QBluez::DevicesModel(DeclarativeManager::self(), parent)
{
}
