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

void DialogGameHistory::addToHistory(std::string str)
{
    ui->History->append(QString::fromStdString(str));
}
