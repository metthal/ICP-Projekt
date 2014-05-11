/*
 * @file mainwindow.h
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
#include "client/clientGame.h"
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

    /**
     * Leaves the actual game, all game objects are destroyed.
     */
    void leaveGame();

private slots:
    /**
     * Connects into server.
     */
    void on_ButtonConnect_clicked();

    /**
     * Shows screen with saved games.
     */
    void on_ButtonLoadGame_clicked();

    /**
     * Shows screen for level selection and game creation.
     */
    void on_ButtonNewGame_clicked();

    /**
     * An event to check if game was joined after period of time.
     */
    void checkGameJoined();

    /**
     * Depends on table being displayed, but generally picks one item
     * from table to work with.
     */
    void on_TableViewGeneral_doubleClicked(const QModelIndex &index);

    /**
     * Returns one screen back.
     */
    void on_ButtonTableViewBack_clicked();

    /**
     * Returns one screen back.
     */
    void on_ButtonLobbyBack_clicked();

    /**
     * Shows screen for level selection.
     */
    void on_ButtonLobbyLevelSelect_clicked();

    /**
     * Checks if the value for stepTime is correct.
     */
    void on_StepTimeValue_editingFinished();

    /**
     * Sends command upon Enter key.
     */
    void on_CommandInput_returnPressed();

    /**
     * Sends command upon button press.
     */
    void on_ButtonSendCommand_clicked();

    /**
     * Shows game menu.
     */
    void on_actionGameEsc_triggered();

    /**
     * Returns one screen back.
     */
    void on_actionMenuEsc_triggered();

    /**
     * Reads all messages from server and processes them.
     * Also updates GUI elements and calls scene redraw.
     */
    void update();

    /**
     * Connects to server by manually specifying IP and port.
     */
    void on_ButtonServerManual_clicked();

    /**
     * Centers the game screen on player of this client.
     */
    void on_actionCenterPlayer_triggered();

    /**
     * Sends command to go left.
     */
    void on_actionGoLeft_triggered();

    /**
     * Sends command to go up.
     */
    void on_actionGoUp_triggered();

    /**
     * Sends command to go right.
     */
    void on_actionGoRight_triggered();

    /**
     * Sends command to go down.
     */
    void on_actionGoDown_triggered();

    /**
     * Sends command to do an action.
     */
    void on_actionGameAction_triggered();

    /**
     * Refreshes the list of servers by sending
     * discovery packet.
     */
    void on_ButtonServerRefresh_clicked();

    /**
     * Returns one screen back.
     */
    void on_ButtonServerBack_clicked();

    /**
     * Shows screen for server selection
     */
    void on_ButtonChangeServer_clicked();

    /**
     * Selects one server from table.
     */
    void on_TableViewServers_doubleClicked(const QModelIndex &index);

    /**
     * An event to check if game was created after period of time.
     */
    void checkGameCreated();

    /**
     * Creates new game and joins it.
     */
    void on_ButtonStartGame_clicked();

    /**
     * Shows game history.
     */
    void on_ButtonCommandHistory_clicked();

private:
    /**
     * Changed page to new one.
     * @param newPage Page to display.
     * @param putInChain Save previous page in previous screens chain?
     * @param object A specific item model with data in case page is table.
     */
    void changePage(QWidget *newPage, bool putInChain = true, void *object = nullptr);

    /**
     * Returns one page back in saved chain.
     */
    void previousPage();

    /**
     * Loads specified page
     * @param page Page to load (GUI elements and initialization).
     * @param object A specific item model with data in case page is table.
     */
    void loadPage(QWidget *page, void *object = nullptr);

    /**
     * Loads page with game scene which is more complex.
     */
    void loadGamePage();

    /**
     * Loads table with specific item model.
     * @param table Table that should be loaded.
     */
    void loadTable(QStandardItemModel *table);

    /**
     * Loads the game itself, all game objects are created.
     * @param playerId Id for this client's player.
     * @param mapData String with data representing the map.
     */
    void loadGame(uint8_t playerId, const std::string &mapData);


    /**
     * Sets in-game message displaying notifications and saves
     * those into history.
     * @param msg Notification to display.
     */
    void setGameMsg(QString msg);

    /**
     * Displays message in form of a dialog.
     * @param msg Message to display.
     */
    void displayMsg(QString msg);

    /**
     * Sends command that has been writte into command edit field.
     */
    void sendCommand();

    /**
     * Handles accidental disconnect from server.
     */
    void handleServerDisconnected();

    /**
     * Converts a tile into position in real 2D coordinates.
     * @param x X tile coordinate.
     * @param y Y tile coordinate.
     * @param xc Centering offset for X axis.
     * @param xy Centering offset for Y axis.
     */
    QPoint tileToPoint(int x, int y, double xc = 0, double yc = 0);

    /**
     * Redraws whole game scene.
     */
    void redrawScene();

    /**
     * Changes color of all pixels in image with specified color.
     * @param pixmap Image.
     * @param origColor Original color that will be changed.
     * @param newColor New color that will replace original.
     */
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
    std::mutex tcpClientMutex;
    uint32_t selectedLevelId;

    int totalCommands;
    int failedCommands;

    ClientGame *game;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastGameMsgTime;
};

#endif // MAINWINDOW_H
