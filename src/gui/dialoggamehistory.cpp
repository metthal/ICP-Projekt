/*
 * @file dialoggamehistory.cpp
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

#include "gui/dialoggamehistory.h"
#include "ui_dialoggamehistory.h"

DialogGameHistory::DialogGameHistory(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogGameHistory)
{
    ui->setupUi(this);
}

DialogGameHistory::~DialogGameHistory()
{
    delete ui;
}

void DialogGameHistory::addToHistory(QString str)
{
    ui->History->append(str);
}
