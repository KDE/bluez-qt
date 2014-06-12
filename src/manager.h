#ifndef QBLUEZ_MANAGER_H
#define QBLUEZ_MANAGER_H

#include <QObject>

#include "qbluez_export.h"

namespace QBluez
{

class AdapterInfo;
class ManagerPrivate;

class QBLUEZ_EXPORT Manager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(Manager* self READ self)
    Q_PROPERTY(bool operational READ isOperational NOTIFY operationalChanged)

public:
    enum RegisterCapability {
        DisplayOnly = 0,
        DisplayYesNo = 1,
        KeyboardOnly = 2,
        NoInputNoOutput = 3
    };

    static Manager *self();
    static void release();

    bool isOperational() const;

Q_SIGNALS:
    void operationalChanged(bool operational);

    void adapterAdded(AdapterInfo *adapter);
    void adapterRemoved(AdapterInfo *adapter);

private:
    explicit Manager();
    ~Manager();

    ManagerPrivate *const d;
    friend class ManagerPrivate;
};

} // namespace QBluez

#endif // QBLUEZ_MANAGER_H
