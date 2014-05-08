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

    void addToHistory(std::string str);

private:
    Ui::DialogGameHistory *ui;
};

#endif // DIALOGGAMEHISTORY_H
