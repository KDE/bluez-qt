#ifndef QBLUEZ_MANAGER_H
#define QBLUEZ_MANAGER_H

#include <QObject>

#include "qbluez_export.h"

namespace QBluez {

class ManagerPrivate;

class QBLUEZ_EXPORT Manager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(Manager* self READ self)
    Q_PROPERTY(bool isBluetoothOperational READ isBluetoothOperational)

public:
    enum RegisterCapability {
        DisplayOnly = 0,
        DisplayYesNo = 1,
        KeyboardOnly = 2,
        NoInputNoOutput = 3
    };

    virtual ~Manager();

    static Manager *self();
    static void release();

    bool isBluetoothOperational() const;

private:
    Manager();

    ManagerPrivate * const d;
};

} // namespace QBluez

#endif // QBLUEZ_MANAGER_H
