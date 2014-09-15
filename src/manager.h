#ifndef QBLUEZ_MANAGER_H
#define QBLUEZ_MANAGER_H

#include <QObject>

#include "qbluez_export.h"

namespace QBluez
{

class Device;
class Adapter;
class Agent;
class PendingCall;
class InitManagerJob;

/**
 * Bluetooth manager.
 *
 * The entry point to communicate with system Bluez daemon.
 *
 * The typical usecase is to work with usableAdapter() (any powered adapter),
 * but it is also possible to use specific adapter.
 *
 * You must call init() before other functions can be used.
 *
 * Example use in C++ code:
 * @code
 * QBluez::Manager *manager = new QBluez::Manager();
 * QBluez::InitManagerJob *job = manager->init();
 * job->start();
 * connect(job, &QBluez::InitManagerJob::result, ...);
 * @endcode
 *
 * In QML, manager is automatically starting initialization after instantiated.
 *
 * There are 2 new signals: initialized() and initializeError(string errorText).

 * Example use in QML code:
 * @code
 * import org.qbluez 1.0 as QBluez
 *
 * QBluez.Manager {
 *     id: manager;
 *
 *     onInitialized: {
 *         console.log("Initialized");
 *     }
 *
 *     onInitializeError: {
 *         console.log("Error: ", errorText);
 *     }
 * }
 * @endcode
 *
 * @note All adapters and devices are owned by manager. You should NOT delete them!
 *
 * @note All communication with Bluez daemon happens asynchronously. Almost all methods
 *       returns PendingCall to help track the call progress and to check for any errors.
 *
 * @note If manager is not operational, all methods that returns a PendingCall
 *       will fail with PendingCall::InternalError.
 *
 * @see InitManagerJob
 */
class QBLUEZ_EXPORT Manager : public QObject
{
    Q_OBJECT

    Q_ENUMS(RegisterCapability)
    Q_PROPERTY(QBluez::Adapter* usableAdapter READ usableAdapter NOTIFY usableAdapterChanged)
    Q_PROPERTY(QList<QBluez::Adapter*> adapters READ adapters)
    Q_PROPERTY(QList<QBluez::Device*> devices READ devices)
    Q_PROPERTY(bool initialized READ isInitialized)
    Q_PROPERTY(bool operational READ isOperational NOTIFY operationalChanged)
    Q_PROPERTY(bool bluetoothOperational READ isBluetoothOperational NOTIFY bluetoothOperationalChanged)

public:
    /**
     * The input/output capabilities of Agent.
     */
    enum RegisterCapability {
        DisplayOnly = 0,
        DisplayYesNo = 1,
        KeyboardOnly = 2,
        NoInputNoOutput = 3
    };

    /**
     * Creates a new Manager object.
     *
     * @param parent
     */
    explicit Manager(QObject *parent = 0);

    /**
     * Destroys a Manager object.
     */
    ~Manager();

    /**
     * Creates a new init job.
     *
     * @return init manager job
     */
    InitManagerJob *init();

    /**
     * Returns whether the manager is initialized.
     *
     * @return true if manager is initialized
     */
    bool isInitialized() const;

    /**
     * Returns whether the manager is operational.
     *
     * The manager is operational when initialization was successful and
     * Bluez system daemon is running.
     *
     * @return true if manager is operational
     */
    bool isOperational() const;

    /**
     * Returns whether Bluetooth is operational.
     *
     * Bluetooth is operational when manager is operational and there is
     * a valid usable adapter.
     *
     * @return true if Bluetooth is operational
     */
    bool isBluetoothOperational() const;

    /**
     * Returns a usable adapter.
     *
     * Usable adapter is any adapter that is currently powered on.
     *
     * @return usable adapter
     */
    Adapter *usableAdapter() const;

    /**
     * Returns a list of all adapters.
     *
     * @return list of adapters
     */
    QList<Adapter*> adapters() const;

    /**
     * Returns a list of all devices.
     *
     * @return list of devices
     */
    QList<Device*> devices() const;

public Q_SLOTS:
    /**
     * Returns an adapter for specified address.
     *
     * @param address address of adapter (eg. "1C:E5:C3:BC:94:7E")
     * @return null if there is no adapter with specified address
     */
    QBluez::Adapter *adapterForAddress(const QString &address) const;

    /**
     * Returns an adapter for specified UBI.
     *
     * @param ubi UBI of adapter (eg. "/org/bluez/hci0")
     * @return null if there is no adapter with specified UBI
     */
    QBluez::Adapter *adapterForUbi(const QString &ubi) const;

    /**
     * Returns a device for specified address.
     *
     * @param address address of device (eg. "40:79:6A:0C:39:75")
     * @return null if there is no device with specified address
     *
     * @note There may be more devices with the same address (same device
     *       in multiple adapters). In this case, the first found device will
     *       be returned.
     */
    QBluez::Device *deviceForAddress(const QString &address) const;

    /**
     * Returns a device for specified UBI.
     *
     * @param ubi UBI of device (eg. "/org/bluez/hci0/dev_40_79_6A_0C_39_75")
     * @return null if there is no device with specified UBI
     */
    QBluez::Device *deviceForUbi(const QString &ubi) const;

    /**
     * Registers agent.
     *
     * This agent will be used for for all actions triggered by the application.
     * Eg. show a PIN code in pairing process.
     *
     * Possible errors: PendingCall::InvalidArguments, PendingCall::AlreadyExists
     *
     * @param agent agent to be registered
     * @param registerCapability agent capability
     * @return void pending call
     */
    PendingCall *registerAgent(Agent *agent, RegisterCapability registerCapability = DisplayYesNo);

    /**
     * Unregisters agent.
     *
     * Possible errors: PendingCall::DoesNotExist
     *
     * @param agent agent to be unregistered
     * @return void pending call
     */
    PendingCall *unregisterAgent(Agent *agent);

    /**
     * Requests default agent.
     *
     * This requests to make the application agent the default agent.
     *
     * Possible errors: PendingCall::DoesNotExist
     *
     * @param agent registered agent
     * @return void pending call
     */
    PendingCall *requestDefaultAgent(Agent *agent);

Q_SIGNALS:
    /**
     * Indicates that operational state have changed.
     */
    void operationalChanged(bool operational);

    /**
     * Indicates that Bluetooth operational state have changed.
     */
    void bluetoothOperationalChanged(bool operational);

    /**
     * Indicates that adapter was added.
     */
    void adapterAdded(QBluez::Adapter *adapter);

    /**
     * Indicates that adapter was removed.
     */
    void adapterRemoved(QBluez::Adapter *adapter);

    /**
     * Indicates that usable adapter have changed.
     */
    void usableAdapterChanged(QBluez::Adapter *adapter);

    /**
     * Indicates that all adapters were removed.
     */
    void allAdaptersRemoved();

private:
    class ManagerPrivate *const d;

    friend class ManagerPrivate;
    friend class InitManagerJobPrivate;
};

} // namespace QBluez

#endif // QBLUEZ_MANAGER_H
