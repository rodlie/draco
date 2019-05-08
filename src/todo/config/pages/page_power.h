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
