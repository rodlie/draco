/*
#
# Draco Desktop Environment <https://dracolinux.org>
# Copyright (c) 2019, Ole-André Rodlie <ole.andre.rodlie@gmail.com> All rights reserved.
#
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>
#
*/

#include "page_power.h"
#include "ui_page_power.h"
#include <QMessageBox>

page_power::page_power(QWidget *parent) : PageWidget(parent), ui(new Ui::page_power())
{
    ui->setupUi(this);
    loading = false;
    power = new PowerSettingsWidget(this);
    ui->verticalLayout->addWidget(power);
}

page_power::~page_power()
{
}

void page_power::SaveSettings()
{
    emit HasPendingChanges(false);
}

void page_power::LoadSettings(int)
{
    emit HasPendingChanges(false);
    emit ChangePageTitle( tr("Power Settings") );
    loading = true;
    loading = false;
}

void page_power::updateIcons()
{
}
