#ifndef QBLUEZ_GETMANAGERJOB_H
#define QBLUEZ_GETMANAGERJOB_H

#include <QObject>

#include "job.h"
#include "qbluez_export.h"

namespace QBluez
{

class Manager;

class QBLUEZ_EXPORT GetManagerJob : public Job
{
    Q_OBJECT

public:
    ~GetManagerJob();

    Manager *manager() const;

Q_SIGNALS:
    void result(GetManagerJob *job);

private:
    explicit GetManagerJob(Manager *manager, QObject *parent = 0);

    void doStart() Q_DECL_OVERRIDE;
    void doEmitResult() Q_DECL_OVERRIDE;

    class GetManagerJobPrivate *d;

    friend class GetManagerJobPrivate;
    friend class Manager;
};

} // namespace QBluez

#endif // QBLUEZ_GETMANAGERJOB_H
