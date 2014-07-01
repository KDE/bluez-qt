#include "loadadaptersjob.h"
#include "manager_p.h"
#include "adapter.h"
#include "adapter_p.h"

namespace QBluez
{

class LoadAdaptersJobPrivate : public QObject
{
    Q_OBJECT

public:
    LoadAdaptersJobPrivate(LoadAdaptersJob *q, ManagerPrivate *manager);

    void doStart();
    void loaded();
    void loadError(const QString &errorText);

    LoadAdaptersJob *q;
    ManagerPrivate *m_manager;
};

LoadAdaptersJobPrivate::LoadAdaptersJobPrivate(LoadAdaptersJob *q, ManagerPrivate *manager)
    : QObject(q)
    , q(q)
    , m_manager(manager)
{
}

void LoadAdaptersJobPrivate::doStart()
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

        connect(adapter->d, &AdapterPrivate::loaded, this, &LoadAdaptersJobPrivate::loaded);
        connect(adapter->d, &AdapterPrivate::loadError, this, &LoadAdaptersJobPrivate::loadError);
    }
}

void LoadAdaptersJobPrivate::loaded()
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

void LoadAdaptersJobPrivate::loadError(const QString &errorText)
{
    q->setError(LoadAdaptersJob::UserDefinedError);
    q->setErrorText(errorText);
    q->emitResult();
}

LoadAdaptersJob::LoadAdaptersJob(ManagerPrivate *manager, QObject *parent)
    : Job(parent)
    , d(new LoadAdaptersJobPrivate(this, manager))
{
}

void LoadAdaptersJob::doStart()
{
    d->doStart();
}

} // namespace QBluez

#include "loadadaptersjob.moc"
