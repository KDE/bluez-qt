#ifndef QBLUEZ_SETPROPERTYJOB_H
#define QBLUEZ_SETPROPERTYJOB_H

#include <QVariant>

#include <QBluez/Job>

namespace QBluez
{

class QBLUEZ_EXPORT SetPropertyJob : public Job
{
    Q_OBJECT

public:
    SetPropertyJob(const QString &name, const QVariant &value, QObject *parent);

Q_SIGNALS:
    void result(Job *job);

private:
    void doStart() Q_DECL_OVERRIDE;

    QString m_name;
    QVariant m_value;
};

} // namespace QBluez

#endif // QBLUEZ_SETPROPERTYJOB_H
