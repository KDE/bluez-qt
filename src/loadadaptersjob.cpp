#include "loadadaptersjob.h"
#include "manager_p.h"
#include "adapter.h"
#include "adapter_p.h"

using namespace QBluez;

LoadAdaptersJob::LoadAdaptersJob(ManagerPrivate *manager, QObject *parent)
    : Job(parent)
    , m_manager(manager)
{
}

void LoadAdaptersJob::doStart()
{
    if (m_manager->m_adaptersLoaded) {
        emitResult();
        return;
    }

    Q_FOREACH (Adapter *adapter, m_manager->m_adapters) {
        adapter->d->load();

        connect(adapter->d, &AdapterPrivate::loaded, [ this ]() {
            Q_FOREACH (Adapter *adapter, m_manager->m_adapters) {
                if (!adapter->d->m_loaded) {
                    return;
                }
            }
            m_manager->m_usableAdapter = m_manager->findUsableAdapter();
            m_manager->m_adaptersLoaded = true;
            emitResult();
        });

        connect(adapter->d, &AdapterPrivate::loadError, [ this ](const QString &error) {
            setError(UserDefinedError);
            setErrorText((error));
            emitResult();
        });
    }
}
