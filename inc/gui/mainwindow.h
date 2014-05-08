#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gui/dialoggamemenu.h"
#include "gui/dialoggamehistory.h"
#include <QMainWindow>
#include <QStandardItemModel>
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QTimer>
#include <chrono>
#include "common/game.h"
#include "client/TcpClient.h"

namespace Ui {

class MainWindow;
}

class PlayerLabel : public QObject
{
    Q_OBJECT

public:
    PlayerLabel(QObject *parent, const Game *game, int id);

protected:
    bool eventFilter(QObject *object, QEvent *event);

private:
    QLabel *label;
    const Game *_game;
    int _id;
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

    void checkGameJoined();

    void on_TableViewGeneral_doubleClicked(const QModelIndex &index);

    void on_ButtonTableViewBack_clicked();

    void on_ButtonLobbyBack_clicked();

    void on_ButtonLobbyLevelSelect_clicked();

    void on_StepTimeValue_editingFinished();

    void on_CommandInput_returnPressed();

    void on_ButtonSendCommand_clicked();

    void on_actionGameEsc_triggered();

    void on_actionMenuEsc_triggered();

    void update();

    void on_ButtonServerManual_clicked();

    void on_actionCenterPlayer_triggered();

    void on_actionGoLeft_triggered();

    void on_actionGoUp_triggered();

    void on_actionGoRight_triggered();

    void on_actionGoDown_triggered();

    void on_actionGameAction_triggered();

    void on_ButtonServerRefresh_clicked();

    void on_ButtonServerBack_clicked();

    void on_ButtonChangeServer_clicked();

    void on_TableViewServers_doubleClicked(const QModelIndex &index);

    void checkGameCreated();

    void on_ButtonStartGame_clicked();

    void on_ButtonCommandHistory_clicked();

private:
    void changePage(QWidget *newPage, bool putInChain = true, void *object = nullptr);

    void previousPage();

    void loadPage(QWidget *page, void *object = nullptr);

    void loadGamePage();

    void loadTable(QStandardItemModel *table);

    void loadGame(uint8_t playerId, const std::string &mapData);

    void setGameMsg(std::string msg);

    void displayMsg(QString msg);

    void sendCommand();

    void handleServerDisconnected();

    QPoint tileToPoint(int x, int y, double xc = 0, double yc = 0);

    void redrawScene();

    void changeColor(QPixmap& pixmap, QColor origColor, QColor newColor);

    DialogGameMenu *gameMenu;
    DialogGameHistory *gameHistory;

    Ui::MainWindow *ui;

    std::vector<int> pages;

    QStandardItemModel *ModelServerSelection;
    QStandardItemModel *ModelLevelSelection;
    QStandardItemModel *ModelSavedGames;
    QStandardItemModel *ModelRunningGames;
    std::vector<uint32_t> tableItemIds;

    static const int maxPlayers = 4;
    PlayerLabel *playerLabels[maxPlayers];
    uint8_t myPlayerId;

    QGraphicsScene *gameScene;

    const int tileTextureSize = 64;
    const int playerTextureHeight = 48;
    const int playerTextureWidth = 32;
    const int sentryTextureHeight = 48;
    const int sentryTextureWidth = 50;
    const int margin = 20;

    QPixmap tileTextures[(int)LevelMap::Tile::Count];
    QPixmap plankTexture;
    QPixmap playerTexture[maxPlayers][(int)Direction::Count];
    QPixmap sentryTexture[(int)Direction::Count];

    QTimer *timer;

    TcpClient *tcpClient;
    uint32_t selectedLevelId;

    Game *game;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastGameMsgTime;
};

#endif // MAINWINDOW_H
