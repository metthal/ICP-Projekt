/*
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
* Marek Milkovič (xmilko01)
* Ivan Ševčík (xsevci50)
*/

#ifndef DIALOGGAMEHISTORY_H
#define DIALOGGAMEHISTORY_H

#include <QDialog>

namespace Ui {
class DialogGameHistory;
}

class DialogGameHistory : public QDialog
{
    Q_OBJECT

public:
    explicit DialogGameHistory(QWidget *parent = 0);
    ~DialogGameHistory();

    void addToHistory(QString str);

private:
    Ui::DialogGameHistory *ui;
};

#endif // DIALOGGAMEHISTORY_H
