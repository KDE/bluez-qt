#ifndef QBLUEZ_LOADADAPTERSJOB_H
#define QBLUEZ_LOADADAPTERSJOB_H

#include <QObject>

#include <QBluez/Job>

#include "qbluez_export.h"

namespace QBluez
{

class ManagerPrivate;

class QBLUEZ_EXPORT LoadAdaptersJob : public Job
{
    Q_OBJECT

public:
    LoadAdaptersJob(ManagerPrivate *manager, QObject *parent = 0);

Q_SIGNALS:
    void result(Job *job);

private:
    void doStart() Q_DECL_OVERRIDE;

    ManagerPrivate *m_manager;
};

} // namespace QBluez

#endif // QBLUEZ_LOADADAPTERSJOB_H
