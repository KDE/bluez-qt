#include "declarativemanager.h"
#include "initmanagerjob.h"

DeclarativeManager::DeclarativeManager(QObject *parent)
    : QBluez::Manager(parent)
{
    QBluez::InitManagerJob *job = init();
    job->start();
    connect(job, &QBluez::InitManagerJob::result, this, &DeclarativeManager::initJobResult);
}

void DeclarativeManager::initJobResult(QBluez::InitManagerJob *job)
{
    if (job->error()) {
        Q_EMIT initializeError(job->errorText());
        return;
    }
    Q_EMIT initialized();
}
