/*
 * BluezQt - Asynchronous Bluez wrapper library
 *
 * Copyright (C) 2014 David Rosca <nowrep@gmail.com>
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

#ifndef DECLARATIVEDEVICESMODEL_H
#define DECLARATIVEDEVICESMODEL_H

#include <QSortFilterProxyModel>

#include "devicesmodel.h"

class DeclarativeManager;

class DeclarativeDevicesModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(DeclarativeManager* manager READ manager WRITE setManager)

public:
    enum DeclarativeDeviceRoles {
        DeviceRole = BluezQt::DevicesModel::LastRole + 1,
        AdapterRole = BluezQt::DevicesModel::LastRole + 2,
        MediaPlayerRole = BluezQt::DevicesModel::LastRole + 3
    };

    explicit DeclarativeDevicesModel(QObject *parent = Q_NULLPTR);

    DeclarativeManager *manager() const;
    void setManager(DeclarativeManager *manager);

    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

private:
    DeclarativeManager *m_manager;
    BluezQt::DevicesModel *m_model;
};

#endif // DECLARATIVEMANAGER_H

