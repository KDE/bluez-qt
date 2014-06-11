#include <Manager>

#include <QDebug>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QBluez::Manager *manager = QBluez::Manager::self();
    qDebug() << manager << manager->isBluetoothOperational();

    return app.exec();
}
