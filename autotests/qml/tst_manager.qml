import QtTest 1.0
import org.kde.bluezqt.fakebluez 1.0

TestCase {
    name: "Manager"

    TestUtils {
        id: utils
    }

    function initTestCase()
    {
        utils.disableDebugOutput();
    }

    function cleanupTestCase()
    {
        utils.enableDebugOutput();
    }

    function cleanup()
    {
        FakeBluez.stop();
    }

    function test_bluezNotRunning()
    {
        var manager = utils.createManager(this);
        var initResult = utils.initManager(manager);

        compare(initResult, "initFinished", "cmp1");
        verify(manager.initialized, "vrf2");
        verify(!manager.operational, "vrf3");
        verify(!manager.bluetoothOperational, "vrf4");

        manager.destroy();
    }

    function test_bluezNotExportingInterfaces()
    {
        FakeBluez.start();
        FakeBluez.runTest("bluez-not-exporting-interfaces");

        var manager = utils.createManager(this);
        var initResult = utils.initManager(manager);

        compare(initResult, "initError", "cmp1");
        verify(!manager.initialized, "vrf2");
        verify(!manager.operational, "vrf3");
        verify(!manager.bluetoothOperational, "vrf4");

        manager.destroy();
    }
}

