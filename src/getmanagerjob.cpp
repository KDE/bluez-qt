#include "getmanagerjob.h"
#include "manager.h"
#include "manager_p.h"
#include "debug_p.h"

namespace QBluez
{

class GetManagerJobPrivate : public QObject
{
    Q_OBJECT

public:
    GetManagerJobPrivate(GetManagerJob *q, Manager *manager);

    void doStart();
    void initError(const QString &errorText);
    void initFinished();

    GetManagerJob *q;
    Manager *m_manager;
};

GetManagerJobPrivate::GetManagerJobPrivate(GetManagerJob *q, Manager *manager)
    : QObject(q)
    , q(q)
    , m_manager(manager)
{
}

void GetManagerJobPrivate::doStart()
{
    if (m_manager) {
        q->emitResult();
        return;
    }

    m_manager = Manager::self();

    connect(m_manager->d, &ManagerPrivate::initError, this, &GetManagerJobPrivate::initError);
    connect(m_manager->d, &ManagerPrivate::initFinished, this, &GetManagerJobPrivate::initFinished);
}

void GetManagerJobPrivate::initError(const QString &errorText)
{
    qCWarning(QBLUEZ) << "GetManagerJob Error:" << errorText;

    q->setError(GetManagerJob::UserDefinedError);
    q->setErrorText(errorText);
    q->emitResult();
}

void GetManagerJobPrivate::initFinished()
{
    q->emitResult();
}

GetManagerJob::GetManagerJob(Manager *manager, QObject *parent)
    : Job(parent)
    , d(new GetManagerJobPrivate(this, manager))
{
}

GetManagerJob::~GetManagerJob()
{
    delete d;
}

Manager *GetManagerJob::manager() const
{
    return d->m_manager;
}

void GetManagerJob::doStart()
{
    d->doStart();
}

void GetManagerJob::doEmitResult()
{
    Q_EMIT result(this);
}

} // namespace QBluez

#include "getmanagerjob.moc"
