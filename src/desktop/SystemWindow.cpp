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

#include "SystemWindow.h"
#include "ui_SystemWindow.h"

#include "LSession.h"
#include <QPoint>
#include <QCursor>
#include <QDebug>
#include <QProcess>
#include <QDesktopWidget>
#include <QMessageBox>

SystemWindow::SystemWindow() : QDialog(), ui(new Ui::SystemWindow)
{
    ui->setupUi(this); // load the designer file
    this->setObjectName("LeaveDialog");
    // Setup the window flags
    this->setWindowFlags( Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    // Setup the icons based on the current theme
    ui->tool_logout->setIcon( LXDG::findIcon("system-log-out","") );
    ui->tool_restart->setIcon( LXDG::findIcon("system-reboot","") );
    ui->tool_shutdown->setIcon( LXDG::findIcon("system-shutdown","") );
    ui->tool_suspend->setIcon( LXDG::findIcon("system-suspend","") );
    ui->push_cancel->setIcon( LXDG::findIcon("system-cancel","dialog-cancel") );
    ui->push_lock->setIcon( LXDG::findIcon("system-lock-screen","") );
    ui->tool_hibernate->setIcon(LXDG::findIcon("system-hibernate"));
    // Connect the signals/slots
    connect(ui->tool_logout, SIGNAL(clicked()), this, SLOT(sysLogout()) );
    connect(ui->tool_restart, SIGNAL(clicked()), this, SLOT(sysRestart()) );
    connect(ui->tool_shutdown, SIGNAL(clicked()), this, SLOT(sysShutdown()) );
    connect(ui->tool_suspend, SIGNAL(clicked()), this, SLOT(sysSuspend()) );
    connect(ui->push_cancel, SIGNAL(clicked()), this, SLOT(sysCancel()) );
    connect(ui->push_lock, SIGNAL(clicked()), this, SLOT(sysLock()) );
    connect(ui->tool_hibernate, SIGNAL(clicked()), this, SLOT(sysHibernate()));

    updateWindow();
    //ui->tool_suspend->setVisible(LSession::handle()->canSuspend());
    connect(QApplication::instance(), SIGNAL(LocaleChanged()), this, SLOT(updateWindow()) );
    connect(QApplication::instance(), SIGNAL(IconThemeChanged()), this, SLOT(updateWindow()) );
}

SystemWindow::~SystemWindow()
{
}

void SystemWindow::updateWindow()
{
    //ui->retranslateUi(this);

    ui->tool_suspend->setEnabled(LSession::handle()->canSuspend());
    ui->tool_hibernate->setEnabled(LSession::handle()->canHibernate());
    ui->tool_restart->setEnabled(LSession::handle()->canReboot());
    ui->tool_shutdown->setEnabled(LSession::handle()->canShutdown());

    // Center this window on the current screen
    QPoint center = QApplication::desktop()->screenGeometry(QCursor::pos()).center(); // get the center of the current screen
    this->move(center.x() - this->width()/2, center.y() - this->height()/2);
}

void SystemWindow::sysLogout()
{
    if (!msgDialog(tr("Logout?"),
                   tr("Are you sure you want to logout?"))) { return; }
    this->close();
    LSession::processEvents();
    QTimer::singleShot(0, LSession::handle(), SLOT(StartLogout()) );
}

void SystemWindow::sysRestart()
{
    if (!msgDialog(tr("Restart computer?"),
                   tr("Are you sure you want to restart the computer?"))) { return; }
    this->close();
    LSession::processEvents();
    LSession::handle()->StartReboot();
}

void SystemWindow::sysShutdown()
{
    if (!msgDialog(tr("Shutdown computer?"),
                   tr("Are you sure you want to shutdown the computer?"))) { return; }
    this->close();
    LSession::processEvents();
    LSession::handle()->StartShutdown();
}

void SystemWindow::sysSuspend()
{
    if (!msgDialog(tr("Suspend computer?"),
                   tr("Are you sure you want to suspend the computer?"))) { return; }
    this->hide();
    LSession::processEvents();
    LSession::handle()->StartSuspend();
}

void SystemWindow::sysHibernate()
{
    if (!msgDialog(tr("Hibernate computer?"),
                   tr("Are you sure you want to hibernate the computer?"))) { return; }
    this->hide();
    LSession::processEvents();
    LSession::handle()->StartSuspend(true);
}

void SystemWindow::sysLock()
{
    this->hide();
    LSession::processEvents();
    LSession::handle()->lockScreen();
}

bool SystemWindow::msgDialog(const QString &title,
                             const QString &text)
{
    if (title.isEmpty() || text.isEmpty()) { return false; }
    int res = QMessageBox::warning(this,
                                   title,
                                   text,
                                   QMessageBox::Yes|QMessageBox::No,
                                   QMessageBox::Yes);
    if (res == QMessageBox::Yes) { return true; }
    return false;
}
