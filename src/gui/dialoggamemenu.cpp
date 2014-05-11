/*
 * @file dialoggamemenu.cpp
 *
 * Project name:
 * Bludiste 2014
 *
 * Description:
 * https://www.fit.vutbr.cz/study/courses/ICP/public/ICP-PRJ-zadani-2014-ija.html
 * https://www.fit.vutbr.cz/study/courses/ICP/public/ICP-PRJ-zadani.html
 *
 * Project's GitHub repository:
 * https://github.com/metthal/ICP-Projekt
 *
 * Team:
 * @author Marek Milkovič (xmilko01)
 * @author Ivan Ševčík (xsevci50)
 */

#include "gui/dialoggamemenu.h"
#include "ui_dialoggamemenu.h"
#include "gui/mainwindow.h"

DialogGameMenu::DialogGameMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogGameMenu)
{
    ui->setupUi(this);
    ui->ButtonSaveGame->hide();
}

DialogGameMenu::~DialogGameMenu()
{
    delete ui;
}

void DialogGameMenu::on_ButtonLeave_clicked()
{
    //TODO disconnect, change page to MainMenu and hide dialog
    MainWindow *main = (MainWindow *)this->parent();
    hide();
    main->leaveGame();
}

void DialogGameMenu::on_ButtonSaveGame_clicked()
{
    //TODO save game and hide dialog
    hide();
}
