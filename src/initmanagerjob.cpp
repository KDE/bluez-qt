#include "initmanagerjob.h"
#include "manager.h"
#include "manager_p.h"
#include "debug_p.h"

namespace QBluez
{

class InitManagerJobPrivate : public QObject
{
public:
    explicit InitManagerJobPrivate(InitManagerJob *q, Manager *manager);

    void doStart();
    void initError(const QString &errorText);
    void initFinished();

    InitManagerJob *q;
    Manager *m_manager;
};

InitManagerJobPrivate::InitManagerJobPrivate(InitManagerJob *q, Manager *manager)
    : QObject(q)
    , q(q)
    , m_manager(manager)
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
    q->emitResult();
}

InitManagerJob::InitManagerJob(Manager *manager)
    : Job(manager)
    , d(new InitManagerJobPrivate(this, manager))
{
}

InitManagerJob::~InitManagerJob()
{
    if (isRunning()) {
        qCWarning(QBLUEZ) << "InitManagerJob Error: Job was deleted before finished!";

        setError(UserDefinedError);
        setErrorText(QStringLiteral("Job was deleted before finished."));
        emitResult();
    }
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
