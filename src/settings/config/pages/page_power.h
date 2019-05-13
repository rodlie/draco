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

#ifndef CONFIG_page_power_H
#define CONFIG_page_power_H

#include "PageWidget.h"
#include "org.dracolinux.Power.SettingsWidget.h"

namespace Ui{
    class page_power;
};

class page_power : public PageWidget{
    Q_OBJECT
public:
    page_power(QWidget *parent);
    ~page_power();

public slots:
    void SaveSettings();
    void LoadSettings(int screennum = -1);
    void updateIcons();

    virtual void settingChanged()
    {
      if (!loading) { emit HasPendingChanges(true); }
    }

private:
    Ui::page_power *ui;
    PowerSettingsWidget *power;
    bool loading;
};

#endif
