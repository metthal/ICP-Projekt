#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dialoggamemenu.h"
#include <QMainWindow>
#include <QStandardItemModel>
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "common/game.h"

namespace Ui {

class MainWindow;
}

class PlayerLabel : public QObject
{
    Q_OBJECT

public:
    PlayerLabel(QObject *parent);

protected:
    bool eventFilter(QObject *object, QEvent *event);

private:
    QLabel *label;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void leaveGame();

private slots:
    void on_ButtonConnect_clicked();

    void on_ButtonLoadGame_clicked();

    void on_ButtonNewGame_clicked();

    void on_TableViewGeneral_doubleClicked(const QModelIndex &index);

    void on_ButtonTableViewBack_clicked();

    void on_ButtonLobbyBack_clicked();

    void on_ButtonLobbyLevelSelect_clicked();

    void on_StepTimeValue_editingFinished();

    void on_CommandInput_returnPressed();

    void on_ButtonSendCommand_clicked();

    void on_actionGameEsc_triggered();

    void on_actionMenuEsc_triggered();

private:
    DialogGameMenu *gameMenu;

    Ui::MainWindow *ui;

    void changePage(QWidget *newPage, bool putInChain = true, void *object = nullptr);

    void previousPage();

    void loadPage(QWidget *page, void *object = nullptr);

    void loadTable(QStandardItemModel *table);

    void loadGame();

    void sendCommand();

    void redrawScene();

    std::vector<QWidget*> pages;

    QStandardItemModel *ModelLevelSelection;
    QStandardItemModel *ModelSavedGames;
    QStandardItemModel *ModelRunningGames;

    static const int maxPlayers = 4;
    PlayerLabel *playerLabels[maxPlayers];

    QGraphicsScene *gameScene;

    const int tileTextureSize = 64;
    const int playerTextureHeight = 48;
    const int playerTextureWidth = 32;
    const int margin = 20;

    QPixmap tileTextures[(int)LevelMap::Tile::Count];
    QPixmap plankTexture;
    QPixmap playerTexture[(int)Direction::Count];

    Game game;
};

#endif // MAINWINDOW_H
