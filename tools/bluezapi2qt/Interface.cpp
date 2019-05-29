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

#include "Interface.h"

Interface::Interface()
{
}

bool Interface::parse(const QString &line)
{
    if (line.startsWith(QStringLiteral("Service\t"))) {
        m_state = State::Service;
    } else if (line.startsWith(QStringLiteral("Interface\t"))) {
        m_state = State::Interface;
    } else if (line.startsWith(QStringLiteral("Object path\t"))) {
        m_state = State::ObjectPath;
    } else if (line.startsWith(QStringLiteral("Methods\t")) || Methods::isMethod(line)) {  // Argh! AgentManager is missing the Methods keyword
        m_state = State::Methods;
    } else if (line.startsWith(QStringLiteral("Properties\t"))) {
        m_state = State::Properties;
    } else if (m_state != State::Comment && !line.isEmpty() && !line.startsWith(QStringLiteral("\t"))) {
        // If we do not parse comment, but line starts with characters, we are done.
        return false;
    }

    switch (m_state) {
    case State::Comment:
        parseComment(line);
        break;
    case State::Service:
        parseService(line);
        break;
    case State::Interface:
        parseInterface(line);
        break;
    case State::ObjectPath:
        parseObjectPath(line);
        break;
    case State::Methods:
        m_methods.parse(line);
        break;
    case State::Properties:
        m_properties.parse(line);
        break;
    }

    return true;
}

bool Interface::finalize()
{
    bool success = true;

    success &= m_methods.finalize();
    success &= m_properties.finalize();

    return success;
}

QStringList Interface::comment() const
{
    return m_comment;
}
QString Interface::service() const
{
    return m_service;
}

QString Interface::name() const
{
    return m_name;
}

QString Interface::objectPath() const
{
    return m_objectPath;
}

Methods Interface::methods() const
{
    return m_methods;
}

Properties Interface::properties() const
{
    return m_properties;
}

void Interface::parseComment(const QString &line)
{
    if (line.isEmpty()) {
        m_comment.append(QString());
        return;
    } else if (line.startsWith(QStringLiteral(" ")) || line.startsWith(QStringLiteral("\t"))) {
        m_comment.append(QString());
    }

    if (!m_comment.last().isEmpty()) {
        m_comment.last() += QStringLiteral(" ");
    }
    m_comment.last() += line;
}

void Interface::parseService(const QString &line)
{
    QRegExp rx(QStringLiteral("Service\\t+(.+)"), Qt::CaseSensitive, QRegExp::RegExp2);
    if (rx.indexIn(line) != -1) {
        m_service = rx.capturedTexts().last();
    }
}

void Interface::parseInterface(const QString &line)
{
    QRegExp rx(QStringLiteral("Interface\\t+(.+)"), Qt::CaseSensitive, QRegExp::RegExp2);
    if (rx.indexIn(line) != -1) {
        m_name = rx.capturedTexts().last();
    }
}

void Interface::parseObjectPath(const QString &line)
{
    QRegExp rx(QStringLiteral("Object path\\t+(.+)"), Qt::CaseSensitive, QRegExp::RegExp2);
    if (rx.indexIn(line) != -1) {
        m_objectPath = rx.capturedTexts().last();
    }
}

