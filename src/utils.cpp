/*****************************************************************************
 * This file is part of the QBluez project                                   *
 *                                                                           *
 * Copyright (C) 2010 Rafael Fernández López <ereslibre@kde.org>             *
 * Copyright (C) 2010 UFO Coders <info@ufocoders.com>                        *
 * Copyright (C) 2014 David Rosca <nowrep@gmail.com>                         *
 *                                                                           *
 * This library is free software; you can redistribute it and/or             *
 * modify it under the terms of the GNU Library General Public               *
 * License as published by the Free Software Foundation; either              *
 * version 2 of the License, or (at your option) any later version.          *
 *                                                                           *
 * This library is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         *
 * Library General Public License for more details.                          *
 *                                                                           *
 * You should have received a copy of the GNU Library General Public License *
 * along with this library; see the file COPYING.LIB.  If not, write to      *
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,      *
 * Boston, MA 02110-1301, USA.                                               *
 *****************************************************************************/

#include "utils.h"

#include <QString>

namespace QBluez
{

QString typeToString(DeviceType type)
{
    switch (type) {
    case Any:
        return QStringLiteral("any");
    case Phone:
        return QStringLiteral("phone");
    case Modem:
        return QStringLiteral("modem");
    case Computer:
        return QStringLiteral("computer");
    case Network:
        return QStringLiteral("network");
    case Headset:
        return QStringLiteral("headset");
    case Headphones:
        return QStringLiteral("headphones");
    case OtherAudio:
        return QStringLiteral("audio");
    case Keyboard:
        return QStringLiteral("keyboard");
    case Mouse:
        return QStringLiteral("mouse");
    case Camera:
        return QStringLiteral("camera");
    case Printer:
        return QStringLiteral("printer");
    case Joypad:
        return QStringLiteral("joypad");
    case Tablet:
        return QStringLiteral("tablet");
    default:
        return QStringLiteral("any");
    }
}

DeviceType stringToType(const QString &stringType)
{
    if (stringType == QLatin1String("any")) {
        return Any;
    } else if (stringType == QLatin1String("phone")) {
        return Phone;
    } else if (stringType == QLatin1String("modem")) {
        return Modem;
    } else if (stringType == QLatin1String("computer")) {
        return Computer;
    } else if (stringType == QLatin1String("network")) {
        return Network;
    } else if (stringType == QLatin1String("headset")) {
        return Headset;
    } else if (stringType == QLatin1String("headphones")) {
        return Headphones;
    } else if (stringType == QLatin1String("audio")) {
        return OtherAudio;
    } else if (stringType == QLatin1String("keyboard")) {
        return Keyboard;
    } else if (stringType == QLatin1String("mouse")) {
        return Mouse;
    } else if (stringType == QLatin1String("camera")) {
        return Camera;
    } else if (stringType == QLatin1String("printer")) {
        return Printer;
    } else if (stringType == QLatin1String("joypad")) {
        return Joypad;
    } else if (stringType == QLatin1String("tablet")) {
        return Tablet;
    }
    return Any;
}

DeviceType classToType(quint32 classNum)
{
    switch ((classNum & 0x1f00) >> 8) {
    case 0x01:
        return Computer;
    case 0x02:
        switch ((classNum & 0xfc) >> 2) {
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x05:
            return Phone;
        case 0x04:
            return Modem;
        }
        break;
    case 0x03:
        return Network;
    case 0x04:
        switch ((classNum & 0xfc) >> 2) {
        case 0x01:
        case 0x02:
            return Headset;
        case 0x06:
            return Headphones;
        default:
            return OtherAudio;
        }
        break;
    case 0x05:
        switch ((classNum & 0xc0) >> 6) {
        case 0x00:
            switch ((classNum & 0x1e) >> 2) {
            case 0x01:
            case 0x02:
                return Joypad;
            }
            break;
        case 0x01:
            return Keyboard;
        case 0x02:
            switch ((classNum & 0x1e) >> 2) {
            case 0x05:
                return Tablet;
            default:
                return Mouse;
            }
        }
        break;
    case 0x06:
        if (classNum & 0x80) {
            return Printer;
        }
        if (classNum & 0x20) {
            return Camera;
        }
        break;
    }

    return Any;
}

} // namespace QBluez
