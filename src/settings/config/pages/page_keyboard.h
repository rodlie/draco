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

#ifndef CONFIG_PAGE_KEYBOARD_H
#define CONFIG_PAGE_KEYBOARD_H

#include "PageWidget.h"
#include <QComboBox>
#include "keyboard_common.h"

namespace Ui{
    class page_keyboard;
};

class page_keyboard : public PageWidget{
    Q_OBJECT
public:
    page_keyboard(QWidget *parent);
    ~page_keyboard();

public slots:
    void SaveSettings();
    void LoadSettings(int screennum = -1);
    void updateIcons();

    virtual void settingChanged()
    {
      if (!loading) { emit HasPendingChanges(true); }
    }

private:
    Ui::page_keyboard *ui;
    bool loading;
    bool verifySettingsReset();

private slots:
    void populateBox(QComboBox *box, xkbType type);
    void populateBoxes();
    void handleLayoutChanged(int index);
    void handleVariantChanged(int index);
    void handleModelChanged(int index);
    void setCurrentIndex(QComboBox *box, xkbType type);
};

#endif
