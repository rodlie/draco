#include "page_keyboard.h"
#include "ui_page_keyboard.h"
#include <QMessageBox>

page_keyboard::page_keyboard(QWidget *parent) : PageWidget(parent), ui(new Ui::page_keyboard())
{
    ui->setupUi(this);
    loading = false;

    ui->keyboardLabel->setPixmap(QIcon::fromTheme("preferences-desktop-keyboard").pixmap(QSize(64, 64)));

    ui->keyboardModelBox->setMaximumWidth(300);
    ui->keyboardLayoutBox->setMaximumWidth(300);
    ui->keybordVariantBox->setMaximumWidth(300);
    ui->keyboardLabel->setMaximumWidth(100);

    connect(ui->keyboardLayoutBox, SIGNAL(currentIndexChanged(int)), this, SLOT(handleLayoutChanged(int)));
    connect(ui->keybordVariantBox, SIGNAL(currentIndexChanged(int)), this, SLOT(handleVariantChanged(int)));
    connect(ui->keyboardModelBox, SIGNAL(currentIndexChanged(int)), this, SLOT(handleModelChanged(int)));
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
    populateBoxes();
    loading = false;
}

void page_keyboard::updateIcons()
{
    ui->keyboardLabel->setPixmap(QIcon::fromTheme("preferences-desktop-keyboard").pixmap(QSize(64, 64)));
}

bool page_keyboard::verifySettingsReset()
{
    return (QMessageBox::Yes ==  QMessageBox::warning(this,
                                                      tr("Verify Settings Reset"),
                                                      tr("Are you sure you want to reset your desktop settings? This change cannot be reversed!"),
                                                      QMessageBox::Yes | QMessageBox::Cancel,
                                                      QMessageBox::Cancel));
}

void page_keyboard::populateBox(QComboBox *box, xkbType type)
{
    if (box==nullptr) { return; }
    QStringList xkb = KeyboardCommon::parseXKB(type);
    box->clear();
    box->addItem(QIcon::fromTheme("keyboard"), QObject::tr("System default"));
    for (int i=0;i<xkb.size();i++) {
        QString itemString = xkb.at(i);
        if (itemString.isEmpty()) { continue; }
        QString itemData = itemString.split(" ").at(0);
        if (itemData.isEmpty()) { continue; }
        box->addItem(QIcon::fromTheme("keyboard"), itemString, itemData);
    }
    setCurrentIndex(box, type);
}

void page_keyboard::populateBoxes()
{
    populateBox(ui->keyboardLayoutBox, xkbLayout);
    populateBox(ui->keybordVariantBox, xkbVariant);
    populateBox(ui->keyboardModelBox, xkbModel);
}

void page_keyboard::handleLayoutChanged(int index)
{
    if (index == -1 || loading) { return; }
    QString value = ui->keyboardLayoutBox->itemData(index).toString();
    KeyboardCommon::saveKeyboard("layout", value);
    KeyboardCommon::loadKeyboard();
}

void page_keyboard::handleVariantChanged(int index)
{
    if (index == -1 || loading) { return; }
    QString value = ui->keybordVariantBox->itemData(index).toString();
    KeyboardCommon::saveKeyboard("variant", value);
    KeyboardCommon::loadKeyboard();
}

void page_keyboard::handleModelChanged(int index)
{
    if (index == -1 || loading) { return; }
    QString value = ui->keyboardModelBox->itemData(index).toString();
    KeyboardCommon::saveKeyboard("model", value);
    KeyboardCommon::loadKeyboard();
}

void page_keyboard::setCurrentIndex(QComboBox *box, xkbType type)
{
    QString kbType;
    switch (type) {
    case xkbLayout:
        kbType = "layout";
        break;
    case xkbVariant:
        kbType = "variant";
        break;
    case xkbModel:
        kbType = "model";
        break;
    }
    QString value = KeyboardCommon::defaultKeyboard(kbType);
    for (int i=0;i<box->count();i++) {
        if (box->itemData(i).toString() == value) { box->setCurrentIndex(i); }
    }
}
