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
