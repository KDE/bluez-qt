#include "initadaptersjob.h"
#include "manager_p.h"
#include "adapter.h"
#include "adapter_p.h"
#include "debug_p.h"

namespace QBluez
{

class InitAdaptersJobPrivate : public QObject
{
    Q_OBJECT

public:
    explicit InitAdaptersJobPrivate(InitAdaptersJob *q, ManagerPrivate *manager);

    void doStart();
    void loaded();
    void loadError(const QString &errorText);

    InitAdaptersJob *q;
    ManagerPrivate *m_manager;
};

InitAdaptersJobPrivate::InitAdaptersJobPrivate(InitAdaptersJob *q, ManagerPrivate *manager)
    : QObject(q)
    , q(q)
    , m_manager(manager)
{
}

void InitAdaptersJobPrivate::doStart()
{
    if (m_manager->m_adaptersLoaded) {
        q->emitResult();
        return;
    }

    if (m_manager->m_adapters.isEmpty()) {
        m_manager->m_adaptersLoaded = true;
        q->emitResult();
        return;
    }

    Q_FOREACH (Adapter *adapter, m_manager->m_adapters) {
        adapter->d->load();

        connect(adapter->d, &AdapterPrivate::loaded, this, &InitAdaptersJobPrivate::loaded);
        connect(adapter->d, &AdapterPrivate::loadError, this, &InitAdaptersJobPrivate::loadError);
    }
}

void InitAdaptersJobPrivate::loaded()
{
    Q_FOREACH (Adapter *adapter, m_manager->m_adapters) {
        if (!adapter->d->m_loaded) {
            return;
        }
    }
    m_manager->m_usableAdapter = m_manager->findUsableAdapter();
    m_manager->m_adaptersLoaded = true;
    q->emitResult();
}

void InitAdaptersJobPrivate::loadError(const QString &errorText)
{
    qCWarning(QBLUEZ) << "LoadAdaptersJob Error:" << errorText;

    q->setError(InitAdaptersJob::UserDefinedError);
    q->setErrorText(errorText);
    q->emitResult();
}

InitAdaptersJob::InitAdaptersJob(ManagerPrivate *manager, QObject *parent)
    : Job(parent)
    , d(new InitAdaptersJobPrivate(this, manager))
{
}

InitAdaptersJob::~InitAdaptersJob()
{
    delete d;
}

void InitAdaptersJob::doStart()
{
    d->doStart();
}

void InitAdaptersJob::doEmitResult()
{
    Q_EMIT result(this);
}

} // namespace QBluez

#include "initadaptersjob.moc"
