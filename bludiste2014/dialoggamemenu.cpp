#include "dialoggamemenu.h"
#include "ui_dialoggamemenu.h"
#include "mainwindow.h"

DialogGameMenu::DialogGameMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogGameMenu)
{
    ui->setupUi(this);
}

DialogGameMenu::~DialogGameMenu()
{
    delete ui;
}

void DialogGameMenu::on_ButtonLeave_clicked()
{
    //TODO disconnect, change page to MainMenu and hide dialog
    MainWindow *main = (MainWindow *)this->parent();
    main->leaveGame();
    hide();
}

void DialogGameMenu::on_ButtonSaveGame_clicked()
{
    //TODO save game and hide dialog
    hide();
}
