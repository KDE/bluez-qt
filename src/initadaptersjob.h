#ifndef QBLUEZ_INITADAPTERSJOB_H
#define QBLUEZ_INITADAPTERSJOB_H

#include <QObject>

#include "job.h"
#include "qbluez_export.h"

namespace QBluez
{

class ManagerPrivate;

class QBLUEZ_EXPORT InitAdaptersJob : public Job
{
    Q_OBJECT

public:
    ~InitAdaptersJob();

Q_SIGNALS:
    void result(InitAdaptersJob *job);

private:
    explicit InitAdaptersJob(ManagerPrivate *manager, QObject *parent = 0);

    void doStart() Q_DECL_OVERRIDE;
    void doEmitResult() Q_DECL_OVERRIDE;

    class InitAdaptersJobPrivate *const d;

    friend class InitAdaptersJobPrivate;
    friend class Manager;
};

} // namespace QBluez

#endif // QBLUEZ_INITADAPTERSJOB_H
