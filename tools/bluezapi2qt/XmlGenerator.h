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

#ifndef XMLGENERATOR_H
#define XMLGENERATOR_H

class BluezApiParser;
class Method;
class Parameter;
class QString;
class QStringList;
class QTextStream;

class XmlGenerator
{
public:
    struct Config {
        bool useOptional = false;
        bool useDeprecated = false;
        bool useExperimental = false;
    };
    XmlGenerator(const Config &config);

    bool generate(const BluezApiParser &parser);

private:
    static void writeHeader(QTextStream &stream);
    static void writeFooter(QTextStream &stream);
    static void writeInterface(QTextStream &stream, const QString &name);
    static void closeInterface(QTextStream &stream);
    static bool writeMethod(QTextStream &stream, const Method &method);
    static bool writeArg(QTextStream &stream, const Parameter &param, const QString &dir);
    static void writeAnnotation(QTextStream &stream, const Parameter &param, const QString &dir, int i);

    Config  m_config;
};

#endif // XMLGENERATOR_H
