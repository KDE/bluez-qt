#include <QDebug>
#include <QTimer>
#include <QCoreApplication>

#include <QBluez/Manager>
#include <QBluez/Adapter>
#include <QBluez/Device>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QBluez::Manager *manager = QBluez::Manager::self();
    qDebug() << manager << manager->isOperational();

    QObject::connect(manager, &QBluez::Manager::operationalChanged, [ = ]() {
        qDebug() << "Operational:" << manager->isOperational()
                 << "Adapters:" << manager->adapters().count()
                 << "Devices:" << manager->devices().count();

        if (!manager->isOperational() || manager->adapters().isEmpty()) {
            return;
        }

        QBluez::Adapter *adapter = manager->adapters().first();
        QBluez::LoadAdapterJob *job = adapter->load();

        job->start();

        QObject::connect(job, &QBluez::Job::result, [ = ](QBluez::Job *) {
            if (job->error() != QBluez::LoadAdapterJob::NoError) {
                qDebug() << "Error loading adapter";
                return;
            }

            qDebug() << "Loaded adapter:";
            qDebug() << "\t Address:" << adapter->address();
            qDebug() << "\t Name:" << adapter->name();
            qDebug() << "\t Alias:" << adapter->alias();
            qDebug() << "\t Class:" << adapter->adapterClass();
            qDebug() << "\t Powered:" << adapter->isPowered();
            qDebug() << "\t Discoverable:" << adapter->isDiscoverable();
            qDebug() << "\t Pairable:" << adapter->isPairable();
            qDebug() << "\t PairableTimeout:" << adapter->pairableTimeout();
            qDebug() << "\t DiscoverableTimeout:" << adapter->discoverableTimeout();
            qDebug() << "\t Discovering:" << adapter->isDiscovering();
            qDebug() << "\t UUIDs:" << adapter->UUIDs();
            qDebug() << "\t Modalias:" << adapter->modalias();
            qDebug() << "\t Devices:" << adapter->devices().count();

            QObject::connect(adapter, &QBluez::Adapter::deviceFound, [ = ](QBluez::Device *device) {
                qDebug() << "Found device:";
                qDebug() << "\t Path:" << device->address();
            });

            QBluez::SetPropertyJob *powerOnJob = adapter->setPowered(true);
            powerOnJob->start();

            QObject::connect(powerOnJob, &QBluez::Job::result, [ = ]() {
                qDebug() << "Starting discovery...";
                adapter->startDiscovery();

                QTimer *timer = new QTimer();
                timer->setSingleShot(true);
                timer->start(10 * 1000);

                QObject::connect(timer, &QTimer::timeout, [ = ]() {
                    qDebug() << "Stopping discovery...";
                    adapter->stopDiscovery();
                });
            });

            QObject::connect(adapter, &QBluez::Adapter::discoveringChanged, [ = ](bool discovering) {
                qDebug() << (discovering ? "Discovery STARTED" : "Discovery STOPPED");
            });
        });
    });

    return app.exec();
}
