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
