import QtTest 1.0
import QtQuick 2.2
import org.kde.bluezqt.testcontroller 1.0

Item {
    SignalSpy {
        id: initSpy
        signalName: "initFinished"
    }

    SignalSpy {
        id: initErrorSpy
        signalName: "initError"
    }

    function createManager(parent)
    {
        var code = "import org.kde.bluezqt 1.0 as BluezQt; BluezQt.Manager { }";
        return Qt.createQmlObject(code, parent, "manager");
    }

    function initManager(manager)
    {
        initSpy.clear();
        initSpy.target = manager;
        initErrorSpy.clear();
        initErrorSpy.target = manager;

        while (initSpy.count == 0 && initErrorSpy.count == 0) {
            wait(200);
        }

        if (initSpy.count > 0) {
            return "initFinished";
        }
        return "initError";
    }

    function disableDebugOutput()
    {
        TestController.disableDebugOutput();
    }

    function enableDebugOutput()
    {
        TestController.enableDebugOutput();
    }
}
