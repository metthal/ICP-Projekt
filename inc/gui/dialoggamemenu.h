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

#ifndef DIALOGGAMEMENU_H
#define DIALOGGAMEMENU_H

#include <QDialog>

namespace Ui {
class DialogGameMenu;
}

class DialogGameMenu : public QDialog
{
    Q_OBJECT

public:
    explicit DialogGameMenu(QWidget *parent = 0);
    ~DialogGameMenu();

private slots:
    /**
     * Leaves game.
     */
    void on_ButtonLeave_clicked();

    /**
     * Saves game.
     */
    void on_ButtonSaveGame_clicked();

private:
    Ui::DialogGameMenu *ui;
};

#endif // DIALOGGAMEMENU_H
