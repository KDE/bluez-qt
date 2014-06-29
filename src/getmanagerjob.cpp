#include "getmanagerjob.h"
#include "manager.h"
#include "manager_p.h"

using namespace QBluez;

GetManagerJob::GetManagerJob(Manager *manager, QObject *parent)
    : Job(parent)
    , m_manager(manager)
{
}

Manager *GetManagerJob::manager() const
{
    return m_manager;
}

void GetManagerJob::doStart()
{
    if (m_manager) {
        emitResult();
        return;
    }

    m_manager = Manager::self();

    connect(m_manager->d, &ManagerPrivate::initError, [ this ](const QString &errorText) {
        setError(UserDefinedError);
        setErrorText(errorText);
        emitResult();
    });

    connect(m_manager->d, &ManagerPrivate::initFinished, [ this ]() {
        emitResult();
    });
}
