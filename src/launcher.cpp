/*
 * Copyright (C) 2021 CutefishOS.
 *
 * Author:     revenmartin <revenmartin@gmail.com>
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

#include "launcher.h"
#include "launcheradaptor.h"
#include "iconthemeimageprovider.h"

#include <QDBusConnection>
#include <QQmlContext>
#include <QScreen>

#include <KWindowSystem>

Launcher::Launcher(QQuickView *w)
  : QQuickView(w)
{
    m_screenRect = qApp->primaryScreen()->geometry();
    m_screenAvailableRect = qApp->primaryScreen()->availableGeometry();

    new LauncherAdaptor(this);

    engine()->rootContext()->setContextProperty("launcher", this);

    setFlags(Qt::FramelessWindowHint);
    setResizeMode(QQuickView::SizeViewToRootObject);
    setClearBeforeRendering(true);
    setScreen(qApp->primaryScreen());
    onGeometryChanged();

    setSource(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    setTitle(tr("Launcher"));
    setVisible(false);

    connect(qApp->primaryScreen(), &QScreen::virtualGeometryChanged, this, &Launcher::onGeometryChanged);
    connect(qApp->primaryScreen(), &QScreen::geometryChanged, this, &Launcher::onGeometryChanged);
    connect(qApp->primaryScreen(), &QScreen::availableGeometryChanged, this, &Launcher::onAvailableGeometryChanged);
    connect(this, &QQuickView::activeChanged, this, &Launcher::onActiveChanged);
}

void Launcher::show()
{
    setVisible(true);
}

void Launcher::hide()
{
    setVisible(false);
}

void Launcher::toggle()
{
    isVisible() ? hide() : show();
}

QRect Launcher::screenRect()
{
    return m_screenRect;
}

QRect Launcher::screenAvailableRect()
{
    return m_screenAvailableRect;
}

void Launcher::onGeometryChanged()
{
    m_screenRect = qApp->primaryScreen()->geometry();
    setGeometry(qApp->primaryScreen()->geometry());

    emit screenRectChanged();
}

void Launcher::onAvailableGeometryChanged(const QRect &geometry)
{
    m_screenAvailableRect = geometry;
    emit screenAvailableGeometryChanged();
}

void Launcher::showEvent(QShowEvent *e)
{
    onGeometryChanged();

    KWindowSystem::setState(winId(), NET::SkipTaskbar | NET::SkipPager);

    QQuickView::showEvent(e);
}

void Launcher::onActiveChanged()
{
    if (!isActive())
        Launcher::hide();
}

