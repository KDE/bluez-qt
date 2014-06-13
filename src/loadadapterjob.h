#ifndef QBLUEZ_LOADADAPTERJOB_H
#define QBLUEZ_LOADADAPTERJOB_H

#include <QObject>

#include <QBluez/Job>

namespace QBluez
{

class AdapterPrivate;

class QBLUEZ_EXPORT LoadAdapterJob : public Job
{
    Q_OBJECT

public:
    LoadAdapterJob(AdapterPrivate *dd, QObject *parent = 0);

Q_SIGNALS:
    void result(Job *job);

private:
    void doStart() Q_DECL_OVERRIDE;

    AdapterPrivate *dd;
};

} // namespace QBluez

#endif // QBLUEZ_LOADADAPTERJOB_H
