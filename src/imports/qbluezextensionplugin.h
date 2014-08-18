#ifndef QBLUEZEXTENSIONPLUGIN_H
#define QBLUEZEXTENSIONPLUGIN_H

#include <QQmlExtensionPlugin>

class QBluezExtensionPlugin: public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.kde.solid")

public:
    QBluezExtensionPlugin(QObject *parent = 0);

    void registerTypes(const char *uri) Q_DECL_OVERRIDE;

};

#endif // QBLUEZEXTENSIONPLUGIN_H

