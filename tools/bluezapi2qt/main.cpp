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

#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

#include "BluezApiParser.h"
#include "XmlGenerator.h"

#define PROGRAMNAME     "bluezapi2qt"
#define PROGRAMVERSION  "0.1"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName(PROGRAMNAME);
    QCoreApplication::setApplicationVersion(PROGRAMVERSION);

    // Add command line parsing
    QCommandLineParser parser;
    parser.setApplicationDescription("Generates D-BUS object introspection XML files out of BlueZ D-Bus API\n"
                                     "description files (*-api.txt).");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("<bluez>-api.txt", "BlueZ D-Bus API description file to use.");
    QCommandLineOption deprecatedOption(QStringList() << "d" << "deprecated", "Generate deprecated methods/properties");
    parser.addOption(deprecatedOption);
    QCommandLineOption experimentalOption(QStringList() << "e" << "experimental", "Generate experimental methods/properties");
    parser.addOption(experimentalOption);
    QCommandLineOption optionalOption(QStringList() << "o" << "optional", "Generate optional methods/properties");
    parser.addOption(optionalOption);
    parser.process(a);

    // Open file
    auto positionalArgs = parser.positionalArguments();
    if (positionalArgs.isEmpty()) {
        fputs(qPrintable(parser.helpText()), stderr);
        return 1;
    }
    auto fileName = positionalArgs.takeFirst();
    QFileInfo inputInfo(fileName);
    if (!inputInfo.exists() || !inputInfo.isFile() || !inputInfo.isReadable()) {
        qCritical() << "Cannot open file" << fileName;
        return 1;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Cannot open file" << file.fileName();
        return 1;
    }
    QTextStream stream(&file);

    // Parse file
    BluezApiParser bluezParser;
    if (!bluezParser.parse(stream)) {
        qCritical() << "Error parsing file" << file.fileName();
        return 1;
    }
    if (!bluezParser.finalize()) {
        qCritical() << "Error parsing file" << file.fileName();
        return 1;
    }

    XmlGenerator::Config xmlConfig;
    xmlConfig.useOptional = parser.isSet(optionalOption);;
    xmlConfig.useDeprecated = parser.isSet(deprecatedOption);;
    xmlConfig.useExperimental = parser.isSet(experimentalOption);;
    XmlGenerator xmlGenerator(xmlConfig);
    if (!xmlGenerator.generate(bluezParser)) {
        qCritical() << "Error generating xml";
        return 1;
    }

    return 0;
}
