/*
#
# Draco Desktop Environment <https://dracolinux.org>
# Copyright (c) 2019, Ole-André Rodlie <ole.andre.rodlie@gmail.com>
# All rights reserved.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>
#
# This file incorporates work covered by the following copyright and
# permission notice:
#
# Lumina Desktop Environment (https://lumina-desktop.org)
# Copyright (c) 2012-2017, Ken Moore (moorekou@gmail.com)
# All rights reserved
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of the organization nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
*/

//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012-2015, Ken Moore
//  Available under the 3-clause BSD license
//===========================================
//  This is the generic class for creating a full-width panel that stays
//    on top of all other windows (top or bottom of the screen only)
//===========================================

#ifndef DESKTOP_PANEL_H
#define DESKTOP_PANEL_H

#include <QWidget>
#include <QBoxLayout>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QMoveEvent>
#include <QDesktopWidget>
#include <QPainter>
#include <QPaintEvent>

#include "panel-plugins/NewPP.h"
#include "panel-plugins/LPPlugin.h"

#include <LuminaX11.h>

class LPanel : public QWidget
{
    Q_OBJECT
private:
    QBoxLayout *layout;
    QSettings *settings;
    QString PPREFIX; // internal prefix for all settings
    QDesktopWidget *screen;
    QWidget *bgWindow, *panelArea;
    QPoint hidepoint, showpoint; // for hidden panels: locations when hidden/visible
    bool defaultpanel, horizontal, hidden, reserveloc;
    QString screenID;
    int panelnum;
    int viswidth, fullwidth;
    QList<LPPlugin*> PLUGINS;
    QRect desiredGeom;
    int Screen(); // Turn the screenID into the appropriate number

public:
    LPanel(QSettings *file,
           QString scr = 0,
           int num = 0,
           QWidget *parent = Q_NULLPTR,
           bool reservespace = true); // settings file, screen number, panel number
    ~LPanel();

    int number()
    {
        return panelnum;
    }

    QString prefix()
    {
        return PPREFIX;
    }

    int visibleWidth()
    {
     return viswidth;
    }
    void prepareToClose();
    void scalePanel(double xscale, double yscale);

public slots:
    void UpdatePanel(bool geomonly = false);  // Load the settings file and update the panel appropriately
    void UpdateLocale(); // Locale Changed externally
    void UpdateTheme(); // Theme Changed externally

private slots:
    void checkPanelFocus();
    void setupFallbackPanel(const QString &panel);

protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
};

#endif
