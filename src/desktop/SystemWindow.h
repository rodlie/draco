/*
#
# Draco Desktop Environment <https://dracolinux.org>
# Copyright (c) 2019, Ole-André Rodlie <ole.andre.rodlie@gmail.com> All rights reserved.
#
# Available under the 3-clause BSD license
# See the LICENSE file for full details
#
*/

#ifndef DESKTOP_SYSTEM_WINDOW_H
#define DESKTOP_SYSTEM_WINDOW_H

#include <QDialog>

#include "ui_SystemWindow.h"

namespace Ui
{
    class SystemWindow;
};

class SystemWindow : public QDialog
{
    Q_OBJECT
    public:
    SystemWindow();
    ~SystemWindow();

    public slots:
    void updateWindow();

    private:
    Ui::SystemWindow *ui;

    //void closeAllWindows();

    private slots:
    void sysLogout();

    void sysRestart();
    //void sysUpdate();
    void sysShutdown();

    void sysSuspend();
    void sysHibernate();

    void sysCancel()
    {
        this->close();
    }

    void sysLock();

    bool msgDialog(QString const &title,
                   QString const &text);
};

#endif
