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
