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
