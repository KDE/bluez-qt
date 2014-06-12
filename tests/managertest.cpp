#include <QDebug>
#include <QTimer>
#include <QCoreApplication>

#include <QBluez/Manager>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QTimer *timer = new QTimer();
    timer->setInterval(1000);
    timer->start();

    QBluez::Manager *manager = QBluez::Manager::self();
    qDebug() << manager << manager->isOperational();

    QObject::connect(timer, &QTimer::timeout, [ = ]() {
        qDebug() << manager->isOperational();
    });

    return app.exec();
}
