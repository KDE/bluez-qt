#ifndef QBLUEZ_INITMANAGERJOB_H
#define QBLUEZ_INITMANAGERJOB_H

#include <QObject>

#include "job.h"
#include "qbluez_export.h"

namespace QBluez
{

class Manager;

class QBLUEZ_EXPORT InitManagerJob : public Job
{
    Q_OBJECT
    Q_PROPERTY(QBluez::Manager* manager READ manager)

public:
    ~InitManagerJob();

    Manager *manager() const;

Q_SIGNALS:
    void result(InitManagerJob *job);

private:
    explicit InitManagerJob(Manager *manager);

    void doStart() Q_DECL_OVERRIDE;
    void doEmitResult() Q_DECL_OVERRIDE;

    class InitManagerJobPrivate *const d;

    friend class InitManagerJobPrivate;
    friend class Manager;
};

} // namespace QBluez

#endif // QBLUEZ_INITMANAGERJOB_H
