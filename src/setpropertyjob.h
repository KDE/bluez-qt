#ifndef QBLUEZ_SETPROPERTYJOB_H
#define QBLUEZ_SETPROPERTYJOB_H

#include <QVariant>

#include "job.h"
#include "qbluez_export.h"

namespace QBluez
{

class QBLUEZ_EXPORT SetPropertyJob : public Job
{
    Q_OBJECT

public:
    SetPropertyJob(const QString &name, const QVariant &value, QObject *parent);
    ~SetPropertyJob();

Q_SIGNALS:
    void result(Job *job);

private:
    void doStart() Q_DECL_OVERRIDE;

    class SetPropertyJobPrivate *d;
    friend class SetPropertyJobPrivate;
};

} // namespace QBluez

#endif // QBLUEZ_SETPROPERTYJOB_H
