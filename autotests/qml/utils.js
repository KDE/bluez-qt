function createManager(parent)
{
    var code = "import org.kde.bluezqt 1.0 as BluezQt; BluezQt.Manager { }";
    return Qt.createQmlObject(code, parent, "manager");
}
