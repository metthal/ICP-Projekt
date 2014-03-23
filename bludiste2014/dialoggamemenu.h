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
    void on_ButtonLeave_clicked();

    void on_ButtonSaveGame_clicked();

private:
    Ui::DialogGameMenu *ui;
};

#endif // DIALOGGAMEMENU_H
