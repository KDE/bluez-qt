#include "fakebluez.h"

#include <iostream>

#include <QDBusConnection>
#include <QCoreApplication>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    if (!QDBusConnection::sessionBus().registerService(QStringLiteral("org.kde.bluezqt.fakebluez"))) {
        std::cerr << "Cannot register org.kde.bluezqt.fakebluez service!" << std::endl;
        return 1;
    }

    new FakeBluez(&app);
    return app.exec();
}
