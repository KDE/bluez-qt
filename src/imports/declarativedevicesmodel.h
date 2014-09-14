#ifndef DECLARATIVEDEVICESMODEL_H
#define DECLARATIVEDEVICESMODEL_H

#include "devicesmodel.h"

class DeclarativeManager;

class DeclarativeDevicesModel : public QBluez::DevicesModel
{
    Q_OBJECT

public:
    DeclarativeDevicesModel(QObject *parent = 0);

};

#endif // DECLARATIVEMANAGER_H

