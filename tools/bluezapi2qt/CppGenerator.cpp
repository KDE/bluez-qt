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

#include "CppGenerator.h"

#include <QDebug>
#include <QFile>

#include "BluezApiParser.h"
#include "TypeAnnotation.h"

CppGenerator::CppGenerator(const Config &config) :
    m_config(config)
{
}

bool CppGenerator::generate(const BluezApiParser &parser)
{
    writeAdaptorHeader(parser);
    writeAdaptorSource(parser);

    return true;
}

void CppGenerator::writeAdaptorHeader(const BluezApiParser &parser)
{
    // Iterate interfaces
    for (const auto &interface : parser.interfaces()) {
        auto className = interfaceToClassName(interface.name());

        // Create file
        QFile file(className.toLower() + QStringLiteral("adaptor.h"));
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qWarning() << "Error opening file for writing:" << file.fileName();
            return;
        }

        // Write content
        QTextStream stream(&file);
        writeCopyrightHeader(stream);
        stream << "#pragma once" << endl << endl;
        stream << "#include <QDBusAbstractAdaptor>" << endl << endl;
        stream << "class QDBusObjectPath;" << endl << endl;
        stream << "namespace BluezQt" << endl << "{" << endl << endl;
        stream << "class " << className << ";" << endl << endl;
        stream << "class " << className << "Adaptor : public QDBusAbstractAdaptor" << endl << "{" << endl;
        stream << "    Q_OBJECT " << endl;
        stream << "    Q_CLASSINFO(\"D-Bus Interface\", \"" << interface.name() << "\")" << endl;

        // Write properties
        for (const auto &property : interface.properties().properties()) {
            // Respect config
            if ((property.tags().isOptional && !m_config.useOptional) ||
                (property.tags().isExperimental && !m_config.useExperimental)) {
                continue;
            }
            stream << "    Q_PROPERTY(" << bluezToQt(property.type()) << " " << property.name() << " READ " << lowerFirstChars(property.name());
            if (!property.tags().isReadOnly) {
                stream << " WRITE set" << property.name();
            }
            stream << ")" << endl;
        }

        stream << endl << "public:" << endl;
        stream << "    explicit " << className << "Adaptor(" << className << "* parent);" << endl << endl;

        // Write property accessors
        for (const auto &property : interface.properties().properties()) {
            // Respect config
            if ((property.tags().isOptional && !m_config.useOptional) ||
                (property.tags().isExperimental && !m_config.useExperimental)) {
                continue;
            }
            stream << "    " << bluezToQt(property.type()) << " " << lowerFirstChars(property.name()) << "() const;" << endl;
            if (!property.tags().isReadOnly) {
                stream << "    void set" << property.name() << "(const " << bluezToQt(property.type()) << " &" << lowerFirstChars(property.name()) << ");" << endl;
            }
            stream << endl;
        }

        stream << "public Q_SLOTS:" << endl;

        // write Methods
        for (const auto &method : interface.methods().methods()) {
            // Respect config
            if ((method.tags().isOptional && !m_config.useOptional) ||
                (method.tags().isExperimental && !m_config.useExperimental)) {
                continue;
            }
            stream << "    " << bluezToQt(method.outParameter().type()) <<  " " << method.name() << "(";
            for (auto it = method.inParameters().begin();
                 it != method.inParameters().end(); ++it) {
                stream << "const " << bluezToQt(it->type()) << " &" << it->name();
                if (it != std::prev(method.inParameters().end())) {
                    stream << ", ";
                }
            }
            stream << ");" << endl;
        }

        // write private members
        stream << endl << "private:" << endl;
        stream << "    " << className << " *m_" << lowerFirstChars(className) << ";" << endl;
        stream << "};" << endl << endl << "} // namespace BluezQt" << endl;

        file.close();
    }
}

void CppGenerator::writeAdaptorSource(const BluezApiParser &parser)
{
    // Iterate interfaces
    for (const auto &interface : parser.interfaces()) {
        auto className = interfaceToClassName(interface.name());

        // Create file
        QFile file(className.toLower() + QStringLiteral("adaptor.cpp"));
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qWarning() << "Error opening file for writing:" << file.fileName();
            return;
        }

        // Write content
        QTextStream stream(&file);
        writeCopyrightHeader(stream);
        stream << "#include \"" << className << "Adaptor.h\"" << endl << endl;
        stream << "#include \"" << className << ".h\"" << endl << endl;
        stream << "namespace BluezQt" << endl << "{" << endl << endl;
        stream << className << "Adaptor::" << className << "Adaptor(" << className << " *parent)" << endl;
        stream << "    : QDBusAbstractAdaptor(parent)" << endl;
        stream << "    , m_" << lowerFirstChars(className) << "(parent)" << endl;
        stream << "{" << endl << "}" << endl << endl;

        // Write property accessors
        for (const auto &property : interface.properties().properties()) {
            // Respect config
            if ((property.tags().isOptional && !m_config.useOptional) ||
                (property.tags().isExperimental && !m_config.useExperimental)) {
                continue;
            }
            stream << bluezToQt(property.type()) << " " << className << "Adaptor::" << lowerFirstChars(property.name()) << "() const" << endl;
            stream << "{" << endl;
            stream << "    return m_" << lowerFirstChars(className) << "->" << lowerFirstChars(property.name()) << "();" << endl;
            stream << "}" << endl << endl;
            if (!property.tags().isReadOnly) {
                stream << "void " << className << "Adaptor::set" << property.name() << "(const " << bluezToQt(property.type()) << " &" << lowerFirstChars(property.name()) << ");" << endl;
                stream << "{" << endl;
                stream << "    m_" << lowerFirstChars(className) << "->set" << property.name() << "(" << lowerFirstChars(property.name()) << ");" << endl;
                stream << "}" << endl << endl;
            }
        }

        // write Methods
        for (const auto &method : interface.methods().methods()) {
            // Respect config
            if ((method.tags().isOptional && !m_config.useOptional) ||
                (method.tags().isExperimental && !m_config.useExperimental)) {
                continue;
            }
            stream << bluezToQt(method.outParameter().type()) << " " << className << "Adaptor::" << method.name() << "(";
            for (auto it = method.inParameters().begin();
                 it != method.inParameters().end(); ++it) {
                stream << "const " << bluezToQt(it->type()) << " &" << it->name();
                if (it != std::prev(method.inParameters().end())) {
                    stream << ", ";
                }
            }
            stream << ")" << endl << "{" << endl;
            stream << "    return m_" << lowerFirstChars(className) << "->" << lowerFirstChars(method.name()) << "(";
            for (auto it = method.inParameters().begin();
                 it != method.inParameters().end(); ++it) {
                stream << it->name();
                if (it != std::prev(method.inParameters().end())) {
                    stream << ", ";
                }
            }
            stream << ");" << endl << "}" << endl << endl;
        }

        stream << "} // namespace BluezQt" << endl;

        file.close();
    }
}

QString CppGenerator::interfaceToClassName(const QString &interface)
{
    auto className = interface.mid(interface.lastIndexOf(QRegExp(QStringLiteral("\\.[A-Z]\\w+")))+1);
    while (className.back() > L'0' && className.back() <= L'9') {
        className.remove(className.size()-1, 1);
    }

    return className;
}

QString CppGenerator::lowerFirstChars(const QString &string)
{
    QString str(string);
    //str.replace(0, 1, string.at(0).toLower());

    QRegExp rx(QStringLiteral("^([A-Z]+)"), Qt::CaseSensitive, QRegExp::RegExp2);
    if (rx.indexIn(string) != -1) {
        QString caps = rx.capturedTexts().last();
        for (int i = 0; i < caps.size()-1; ++i) {
            str.replace(i, 1, str.at(i).toLower());
        }
    }
    str.replace(0, 1, string.at(0).toLower());
    str.replace(string.size()-1, 1, string.at(string.size()-1).toLower());

    return str;
}

void CppGenerator::writeCopyrightHeader(QTextStream &stream)
{
    stream << "/*" << endl;
    stream << " * BluezQt - Asynchronous Bluez wrapper library" << endl;
    stream << " *" << endl;
    stream << " * Copyright (C) " << m_config.year << " " << m_config.author << endl;
    stream << " *" << endl;
    stream << " * This library is free software; you can redistribute it and/or" << endl;
    stream << " * modify it under the terms of the GNU Lesser General Public" << endl;
    stream << " * License as published by the Free Software Foundation; either" << endl;
    stream << " * version 2.1 of the License, or (at your option) version 3, or any" << endl;
    stream << " * later version accepted by the membership of KDE e.V. (or its" << endl;
    stream << " * successor approved by the membership of KDE e.V.), which shall" << endl;
    stream << " * act as a proxy defined in Section 6 of version 3 of the license." << endl;
    stream << " *" << endl;
    stream << " * This library is distributed in the hope that it will be useful," << endl;
    stream << " * but WITHOUT ANY WARRANTY; without even the implied warranty of" << endl;
    stream << " * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU" << endl;
    stream << " * Lesser General Public License for more details." << endl;
    stream << " *" << endl;
    stream << " * You should have received a copy of the GNU Lesser General Public" << endl;
    stream << " * License along with this library. If not, see <http://www.gnu.org/licenses/>." << endl;
    stream << " */" << endl << endl;
}
