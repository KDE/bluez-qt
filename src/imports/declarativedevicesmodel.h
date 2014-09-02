#ifndef DECLARATIVEDEVICESMODEL_H
#define DECLARATIVEDEVICESMODEL_H

#include "devicesmodel.h"

class DeclarativeManager;

class DeclarativeDevicesModel : public QBluez::DevicesModel
{
    Q_OBJECT
    Q_PROPERTY(DeclarativeManager* manager READ manager WRITE setManager)

public:
    DeclarativeDevicesModel(QObject *parent = 0);

    DeclarativeManager *manager() const;
    void setManager(DeclarativeManager *manager);

private:
    DeclarativeManager *m_manager;

};

#endif // DECLARATIVEMANAGER_H

