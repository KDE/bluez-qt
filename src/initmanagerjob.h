#ifndef QBLUEZ_INITMANAGERJOB_H
#define QBLUEZ_INITMANAGERJOB_H

#include <QObject>

#include "job.h"
#include "qbluez_export.h"

namespace QBluez
{

class Manager;

/**
 * Init manager job.
 *
 * This class represents a job that initializes Manager.
 */
class QBLUEZ_EXPORT InitManagerJob : public Job
{
    Q_OBJECT
    Q_PROPERTY(QBluez::Manager* manager READ manager)

public:
    /**
      * Destroys an InitManagerJob object.
      */
    ~InitManagerJob();

    /**
     * Returns a manager that is being initialized.
     *
     * @return manager
     */
    Manager *manager() const;

Q_SIGNALS:
    /**
     * Indicates that the job have finished.
     */
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
