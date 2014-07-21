#include "initobexmanagerjob.h"
#include "obexmanager.h"
#include "obexmanager_p.h"
#include "debug_p.h"

namespace QBluez
{

class InitObexManagerJobPrivate : public QObject
{
    Q_OBJECT

public:
    explicit InitObexManagerJobPrivate(InitObexManagerJob *q, ObexManager *manager);

    void doStart();
    void initError(const QString &errorText);
    void initFinished();

    InitObexManagerJob *q;
    ObexManager *m_manager;
};

InitObexManagerJobPrivate::InitObexManagerJobPrivate(InitObexManagerJob *q, ObexManager *manager)
    : QObject(q)
    , q(q)
    , m_manager(manager)
{
}

void InitObexManagerJobPrivate::doStart()
{
    if (m_manager->d->m_initialized) {
        qCWarning(QBLUEZ) << "InitObexManagerJob: ObexManager already initialized!";
        q->emitResult();
        return;
    }

    connect(m_manager->d, &ObexManagerPrivate::initError, this, &InitObexManagerJobPrivate::initError);
    connect(m_manager->d, &ObexManagerPrivate::initFinished, this, &InitObexManagerJobPrivate::initFinished);

    m_manager->d->init();
}

void InitObexManagerJobPrivate::initError(const QString &errorText)
{
    qCWarning(QBLUEZ) << "InitObexManagerJob Error:" << errorText;

    q->setError(InitObexManagerJob::UserDefinedError);
    q->setErrorText(errorText);
    q->emitResult();
}

void InitObexManagerJobPrivate::initFinished()
{
    q->emitResult();
}

InitObexManagerJob::InitObexManagerJob(ObexManager *manager)
    : Job(manager)
    , d(new InitObexManagerJobPrivate(this, manager))
{
}

InitObexManagerJob::~InitObexManagerJob()
{
    delete d;
}

ObexManager *InitObexManagerJob::manager() const
{
    return d->m_manager;
}

void InitObexManagerJob::doStart()
{
    d->doStart();
}

void InitObexManagerJob::doEmitResult()
{
    Q_EMIT result(this);
}

} // namespace QBluez

#include "initobexmanagerjob.moc"
