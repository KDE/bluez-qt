#ifndef DECLARATIVEMANAGER_H
#define DECLARATIVEMANAGER_H

#include "manager.h"

class DeclarativeManager : public QBluez::Manager
{
    Q_OBJECT

public:
    DeclarativeManager(QObject *parent = 0);

Q_SIGNALS:
    void initialized();
    void initializeError(const QString &errorText);

private:
    void initJobResult(QBluez::InitManagerJob *job);
};

#endif // DECLARATIVEMANAGER_H

