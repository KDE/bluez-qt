/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * Copyright (C) 2019 Manuel Weichselbaumer <mincequi@web.de>
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

#include "XmlGenerator.h"

#include <QDebug>
#include <QFile>

#include "BluezApiParser.h"
#include "TypeAnnotation.h"

XmlGenerator::XmlGenerator(const Config &config) :
    m_config(config)
{
}

bool XmlGenerator::generate(const BluezApiParser &parser)
{
    // Iterate interfaces
    for (const auto &interface : parser.interfaces()) {
        // Only consider interfaces with methods
        if (interface.methods().methods().empty()) {
            continue;
        }

        // Create file
        QFile file(interface.name() + QStringLiteral(".xml"));
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qWarning() << "Error opening file for writing:" << file.fileName();
            return false;
        }

        // Write content
        QTextStream stream(&file);
        writeHeader(stream);
        writeInterface(stream, interface.name());

        // Iterate methods
        for (const auto &method : interface.methods().methods()) {
            // Respect config
            if ((method.tags().isDeprecated && !m_config.useDeprecated) ||
                (method.tags().isOptional && !m_config.useOptional) ||
                (method.tags().isExperimental && !m_config.useExperimental)) {
                continue;
            }

            writeMethod(stream, method);
        }

        closeInterface(stream);
        writeFooter(stream);
        file.close();
    }

    return true;
}

void XmlGenerator::writeHeader(QTextStream &stream)
{
    stream << "<?xml version=\"1.0\"?>" << endl;
    stream << "<!DOCTYPE node PUBLIC \"-//freedesktop//DTD D-BUS Object Introspection 1.0//EN\" \"http://www.freedesktop.org/standards/dbus/1.0/introspect.dtd\">" << endl;
    stream << "<node>" << endl;
}

void XmlGenerator::writeFooter(QTextStream &stream)
{
    stream << "</node>" << endl;
}

void XmlGenerator::writeInterface(QTextStream &stream, const QString &name)
{
    stream << "  <interface name=\"" << name << "\">" << endl;
}

void XmlGenerator::closeInterface(QTextStream &stream)
{
    stream << "  </interface>" << endl;
}

bool XmlGenerator::writeMethod(QTextStream &stream, const Method &method)
{
    stream << "    <method name=\"" << method.name()  << "\"";

    // Some beautification
    if (method.inParameters().empty() && method.outParameters().empty()) {
        stream << "/>" << endl;
        return true;
    }

    stream << ">" << endl;

    for (const auto &param : method.inParameters()) {
        if (!writeArg(stream, param, QStringLiteral("in"))) {
            return false;
        }
    }
    for (const auto &param : method.outParameters()) {
        if (!writeArg(stream, param, QStringLiteral("out"))) {
            return false;
        }
    }
    for (int i = 0; i < method.inParameters().size(); ++i) {
        writeAnnotation(stream, method.inParameters().at(i), QStringLiteral("In"), i);
    }
    for (int i = 0; i < method.outParameters().size(); ++i) {
        writeAnnotation(stream, method.outParameters().at(i), QStringLiteral("Out"), i);
    }

    stream << "    </method>" << endl;

    return true;
}

bool XmlGenerator::writeArg(QTextStream &stream, const Parameter &param, const QString &dir)
{
    auto dbusType = annotateType(AnnotationType::Bluez, AnnotationType::Dbus, param.type());
    if (dbusType.isEmpty()) {
        return false;
    }
    stream << "      <arg name=\"" << param.name() << "\" type=\"" << dbusType <<
              "\" direction=\"" << dir << "\"/>" << endl;

    return true;
}

void XmlGenerator::writeAnnotation(QTextStream &stream, const Parameter &param, const QString &dir, int i)
{
    auto qtType = annotateType(AnnotationType::Bluez, AnnotationType::Qt, param.type());
    if (qtType.isEmpty()) {
        return;
    }
    stream << "      <annotation name=\"org.qtproject.QtDBus.QtTypeName." << dir <<
              QString::number(i) << "\" value=\"" << qtType << "\"/>" << endl;

    return;
}
