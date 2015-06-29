/*
 * Copyright (C) 2015 David Rosca <nowrep@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

import QtTest 1.0
import QtQuick 2.2
import org.kde.bluezqt.fakebluez 1.0
import org.kde.bluezqt 1.0 as BluezQt

TestCase {
    name: "Manager"

    function cleanup()
    {
        FakeBluez.stop();
    }

    function test_bluezNotRunning()
    {
        var manager = BluezQt.Manager;
        tryCompare(manager, "operational", false);

        verify(!manager.operational, "operational");
        verify(!manager.bluetoothOperational, "btOperational");
    }

    function test_bluezNotExportingInterfaces()
    {
        FakeBluez.start();
        FakeBluez.runTest("bluez-not-exporting-interfaces");

        var manager = BluezQt.Manager;
        tryCompare(manager, "operational", false);

        verify(!manager.operational, "operational");
        verify(!manager.bluetoothOperational, "btOperational");
    }

    function test_bluezNoAdapters()
    {
        FakeBluez.start();
        FakeBluez.runTest("bluez-no-adapters");

        var manager = BluezQt.Manager;
        tryCompare(manager, "operational", true);

        verify(manager.operational, "operational");
        verify(!manager.bluetoothOperational, "btOperational");
    }

    SignalSpy {
        id: usableAdapterChangedSpy
        signalName: "usableAdapterChanged"
    }

    function test_usableAdapter()
    {
        FakeBluez.start();
        FakeBluez.runTest("bluez-standard");

        var adapter1path = "/org/bluez/hci0";
        var adapter1props = {
            Path: adapter1path,
            Address: "1C:E5:C3:BC:94:7E",
            Name: "TestAdapter",
            Powered: false,
            _toDBusObjectPath: [ "Path" ]
        }
        FakeBluez.runAction("devicemanager", "create-adapter", adapter1props);

        var adapter2path = "/org/bluez/hci1";
        var adapter2props = {
            Path: adapter2path,
            Address: "2E:3A:C3:BC:85:7C",
            Name: "TestAdapter2",
            Powered: false,
            _toDBusObjectPath: [ "Path" ]
        }
        FakeBluez.runAction("devicemanager", "create-adapter", adapter2props);


        var manager = BluezQt.Manager;

        tryCompare(manager, "operational", true);
        verify(!manager.bluetoothOperational, "btOperational");

        usableAdapterChangedSpy.target = manager;

        var properties = {
            Path: adapter1path,
            Name: "Powered",
            Value: true,
            _toDBusObjectPath: [ "Path" ]
        }
        FakeBluez.runAction("devicemanager", "change-adapter-property", properties);

        tryCompare(usableAdapterChangedSpy, "count", 1);
        compare(manager.usableAdapter.ubi, adapter1path);

        usableAdapterChangedSpy.clear();

        properties.Value = false;
        FakeBluez.runAction("devicemanager", "change-adapter-property", properties);

        tryCompare(usableAdapterChangedSpy, "count", 1);
        compare(manager.usableAdapter, null);

        usableAdapterChangedSpy.clear();

        properties.Path = adapter2path;
        properties.Value = true;
        FakeBluez.runAction("devicemanager", "change-adapter-property", properties);

        tryCompare(usableAdapterChangedSpy, "count", 1);
        compare(manager.usableAdapter.ubi, adapter2path);
    }

    SignalSpy {
        id: managerAdapterRemovedSpy
        signalName: "adapterRemoved"
    }

    SignalSpy {
        id: managerDeviceRemovedSpy
        signalName: "deviceRemoved"
    }

    function test_adapterWithDevicesRemoved()
    {
        FakeBluez.start();
        FakeBluez.runTest("bluez-standard");

        var adapter1path = "/org/bluez/hci0";
        var adapterProps = {
            Path: adapter1path,
            Address: "1C:E5:C3:BC:94:7E",
            Name: "TestAdapter",
            Powered: false,
            _toDBusObjectPath: [ "Path" ]
        }
        FakeBluez.runAction("devicemanager", "create-adapter", adapterProps);

        var device1props = {
            Path: "/org/bluez/hci0/dev_40_79_6A_0C_39_75",
            Adapter: adapter1path,
            Address: "40:79:6A:0C:39:75",
            Name: "TestDevice",
            _toDBusObjectPath: [ "Path", "Adapter" ]
        }
        FakeBluez.runAction("devicemanager", "create-device", device1props);

        var device2props = {
            Path: "/org/bluez/hci0/dev_50_79_6A_0C_39_75",
            Adapter: adapter1path,
            Address: "50:79:6A:0C:39:75",
            Name: "TestDevice2",
            _toDBusObjectPath: [ "Path", "Adapter" ]
        }
        FakeBluez.runAction("devicemanager", "create-device", device2props);

        var manager = BluezQt.Manager;

        tryCompare(manager, "operational", true);

        var adapter = manager.adapterForAddress("1C:E5:C3:BC:94:7E");
        var device1 = manager.deviceForAddress("40:79:6A:0C:39:75");
        var device2 = manager.deviceForAddress("50:79:6A:0C:39:75");

        verify(adapter);
        verify(device1);
        verify(device2);

        managerAdapterRemovedSpy.target = manager;
        managerDeviceRemovedSpy.target = manager;

        var properties = {
            Path: adapter1path,
            _toDBusObjectPath: [ "Path" ]
        }
        FakeBluez.runAction("devicemanager", "remove-adapter", properties);

        tryCompare(managerAdapterRemovedSpy, "count", 1);
        tryCompare(managerDeviceRemovedSpy, "count", 2);
        compare(manager.adapters.length, 0);
        compare(manager.devices.length, 0);
    }
}

