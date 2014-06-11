#ifndef QBLUEZ_MANAGER_P_H
#define QBLUEZ_MANAGER_P_H

#include <QObject>

namespace QBluez {

class ManagerPrivate : public QObject
{
    Q_OBJECT

public:
    explicit ManagerPrivate(QObject *parent = 0);

    void initialize();

private Q_SLOTS:

private:
    bool m_bluezRunning;

};

} // namespace QBluez

#endif // QBLUEZ_MANAGER_P_H
