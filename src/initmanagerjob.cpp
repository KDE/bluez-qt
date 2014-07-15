#include "initmanagerjob.h"
#include "initadaptersjob.h"
#include "manager.h"
#include "manager_p.h"
#include "debug_p.h"

namespace QBluez
{

class InitManagerJobPrivate : public QObject
{
    Q_OBJECT

public:
    InitManagerJobPrivate(InitManagerJob *q, Manager *manager, Manager::InitType initType);

    void doStart();
    void initError(const QString &errorText);
    void initFinished();

    InitManagerJob *q;
    Manager *m_manager;
    Manager::InitType m_initType;
};

InitManagerJobPrivate::InitManagerJobPrivate(InitManagerJob *q, Manager *manager, Manager::InitType initType)
    : QObject(q)
    , q(q)
    , m_manager(manager)
    , m_initType(initType)
{
}

void InitManagerJobPrivate::doStart()
{
    if (m_manager->d->m_initialized) {
        qCWarning(QBLUEZ) << "InitManagerJob: Manager already initialized!";
        q->emitResult();
        return;
    }

    m_manager->d->init();
    connect(m_manager->d, &ManagerPrivate::initError, this, &InitManagerJobPrivate::initError);
    connect(m_manager->d, &ManagerPrivate::initFinished, this, &InitManagerJobPrivate::initFinished);
}

void InitManagerJobPrivate::initError(const QString &errorText)
{
    qCWarning(QBLUEZ) << "GetManagerJob Error:" << errorText;

    q->setError(InitManagerJob::UserDefinedError);
    q->setErrorText(errorText);
    q->emitResult();
}

void InitManagerJobPrivate::initFinished()
{
    if (m_initType == Manager::InitManagerOnly) {
        q->emitResult();
    } else if (m_initType == Manager::InitManagerAndAdapters) {
        InitAdaptersJob *job = m_manager->initAdapters();
        job->start();
        connect(job, &InitAdaptersJob::result, [ this ](InitAdaptersJob *job) {
            if (job->error()) {
                q->setError(InitManagerJob::UserDefinedError);
                q->setErrorText(job->errorText());
            }
            q->emitResult();
        });
    }
}

InitManagerJob::InitManagerJob(Manager *manager, Manager::InitType initType)
    : Job(manager)
    , d(new InitManagerJobPrivate(this, manager, initType))
{
}

InitManagerJob::~InitManagerJob()
{
    delete d;
}

Manager *InitManagerJob::manager() const
{
    return d->m_manager;
}

void InitManagerJob::doStart()
{
    d->doStart();
}

void InitManagerJob::doEmitResult()
{
    Q_EMIT result(this);
}

} // namespace QBluez

#include "initmanagerjob.moc"
