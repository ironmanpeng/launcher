/*
 * Copyright (C) 2021 CutefishOS.
 *
 * Author:     Kate Leet <kate@cutefishos.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "appmanager.h"
#include <QFile>

AppManager::AppManager(QObject *parent)
    : QObject(parent)
    , m_iface("com.cutefish.Daemon",
              "/AppManager",
              "com.cutefish.AppManager", QDBusConnection::sessionBus())
{

}

void AppManager::uninstall(const QString &desktopFile)
{
    if (m_iface.isValid()) {
        m_iface.asyncCall("uninstall", desktopFile);
    }
}

bool AppManager::isCutefishOS()
{
    return QFile::exists("/etc/cutefishos");
}
