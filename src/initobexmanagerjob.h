#ifndef QBLUEZ_INITOBEXMANAGERJOB_H
#define QBLUEZ_INITOBEXMANAGERJOB_H

#include <QObject>

#include "job.h"
#include "qbluez_export.h"

namespace QBluez
{

class ObexManager;

class QBLUEZ_EXPORT InitObexManagerJob : public Job
{
    Q_OBJECT
    Q_PROPERTY(QBluez::ObexManager* manager READ manager)

public:
    ~InitObexManagerJob();

    ObexManager *manager() const;

Q_SIGNALS:
    void result(InitObexManagerJob *job);

private:
    explicit InitObexManagerJob(ObexManager *manager);

    void doStart() Q_DECL_OVERRIDE;
    void doEmitResult() Q_DECL_OVERRIDE;

    class InitObexManagerJobPrivate *const d;

    friend class InitObexManagerJobPrivate;
    friend class ObexManager;
};

} // namespace QBluez

#endif // QBLUEZ_INITOBEXMANAGERJOB_H
