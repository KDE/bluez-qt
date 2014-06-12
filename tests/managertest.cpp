#include <QDebug>
#include <QCoreApplication>

#include <QBluez/Manager>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QBluez::Manager *manager = QBluez::Manager::self();
    qDebug() << manager << manager->isOperational();

    QObject::connect(manager, &QBluez::Manager::operationalChanged, [ = ]() {
        qDebug() << "Operational:" << manager->isOperational()
                 << "Adapters:" << manager->adapters().count()
                 << "Devices:" << manager->devices().count();
    });

    return app.exec();
}
