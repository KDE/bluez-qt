import QtTest 1.0
import org.kde.bluezqt.fakebluez 1.0
import org.kde.bluezqt.testcontroller 1.0
import "utils.js" as Utils

TestCase {
    name: "Manager"

    function initTestCase()
    {
        TestController.disableDebugOutput();
    }

    function cleanupTestCase()
    {
        TestController.enableDebugOutput();
    }

    function cleanup()
    {
        FakeBluez.stop();
    }

    SignalSpy {
        id: initSpy
        signalName: "initFinished"
    }

    SignalSpy {
        id: initErrorSpy
        signalName: "initError"
    }

    function test_bluezNotRunning()
    {
        var manager = Utils.createManager(this);
        initSpy.target = manager;

        tryCompare(initSpy, "count", 1);
        verify(manager.initialized);
        verify(!manager.operational);
        verify(!manager.bluetoothOperational);

        manager.destroy();
    }

    function test_bluezNotExportingInterfaces()
    {
        FakeBluez.start();
        FakeBluez.runTest("bluez-not-exporting-interfaces");

        var manager = Utils.createManager(this);
        initErrorSpy.target = manager;

        tryCompare(initErrorSpy, "count", 1);
        verify(!manager.initialized);
        verify(!manager.operational);
        verify(!manager.bluetoothOperational);

        manager.destroy();
    }
}

