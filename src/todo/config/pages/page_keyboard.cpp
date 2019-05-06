#include "page_keyboard.h"
#include "ui_page_keyboard.h"
#include <QMessageBox>

page_keyboard::page_keyboard(QWidget *parent) : PageWidget(parent), ui(new Ui::page_keyboard())
{
    ui->setupUi(this);
    loading = false;
}

page_keyboard::~page_keyboard()
{
}

void page_keyboard::SaveSettings()
{
    emit HasPendingChanges(false);
}

void page_keyboard::LoadSettings(int)
{
    emit HasPendingChanges(false);
    emit ChangePageTitle( tr("Keyboard Settings") );
    loading = true;
    loading = false;
}

void page_keyboard::updateIcons()
{
}

bool page_keyboard::verifySettingsReset()
{
    return (QMessageBox::Yes ==  QMessageBox::warning(this,
                                                      tr("Verify Settings Reset"),
                                                      tr("Are you sure you want to reset your desktop settings? This change cannot be reversed!"),
                                                      QMessageBox::Yes | QMessageBox::Cancel,
                                                      QMessageBox::Cancel));
}
