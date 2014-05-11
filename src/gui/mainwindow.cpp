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

#include "gui/mainwindow.h"
#include "ui_mainwindow.h"
#include "gui/dialoggamemenu.h"
#include "gui/dialoggamehistory.h"
#include "common/Opcode.h"
#include "common/msgexception.h"
#include <QDateTime>
#include <QHeaderView>
#include <QScrollBar>
#include <QToolTip>
#include <QBitmap>
#include <QPainter>
#include <QAbstractItemView>
#include <QMessageBox>
#include <QValidator>
#include <QGraphicsSimpleTextItem>

QString formatTime(int seconds)
{
    int hours = seconds / 3600;
    seconds -= hours * 3600;
    int minutes = seconds / 60;
    seconds -= minutes * 60;
    return QString::number(hours) + (minutes < 10 ? ":0" : ":") + QString::number(minutes) + (seconds < 10 ? ":0" : ":") + QString::number(seconds);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->GameView->horizontalScrollBar()->setStyleSheet("QScrollBar {height:0px;}");
    ui->GameView->horizontalScrollBar()->hide();
    ui->GameView->verticalScrollBar()->setStyleSheet("QScrollBar {width:0px;}");
    ui->GameView->verticalScrollBar()->hide();

    loadPage(ui->PageMainMenu);
    gameScene = nullptr;

    ModelServerSelection = new QStandardItemModel(0, 2, this);
    ModelServerSelection->setHorizontalHeaderItem(0, new QStandardItem(QString("IP")));
    ModelServerSelection->setHorizontalHeaderItem(1, new QStandardItem(QString("Name")));
    ModelServerSelection->setHorizontalHeaderItem(2, new QStandardItem(QString("Number of games")));
    ui->TableViewServers->setModel(ModelServerSelection);

    ModelLevelSelection = new QStandardItemModel(0, 2, this);
    ModelLevelSelection->setHorizontalHeaderItem(0, new QStandardItem(QString("Name")));
    ModelLevelSelection->setHorizontalHeaderItem(1, new QStandardItem(QString("Size")));

    ModelSavedGames = new QStandardItemModel(0, 5, this);
    ModelSavedGames->setHorizontalHeaderItem(0, new QStandardItem(QString("Date")));
    ModelSavedGames->setHorizontalHeaderItem(1, new QStandardItem(QString("Name")));
    ModelSavedGames->setHorizontalHeaderItem(2, new QStandardItem(QString("Level")));
    ModelSavedGames->setHorizontalHeaderItem(3, new QStandardItem(QString("Size")));
    ModelSavedGames->setHorizontalHeaderItem(4, new QStandardItem(QString("Speed")));

    ModelRunningGames = new QStandardItemModel(0, 6, this);
    ModelRunningGames->setHorizontalHeaderItem(0, new QStandardItem(QString("Game name")));
    ModelRunningGames->setHorizontalHeaderItem(1, new QStandardItem(QString("Map name")));
    ModelRunningGames->setHorizontalHeaderItem(2, new QStandardItem(QString("Size")));
    ModelRunningGames->setHorizontalHeaderItem(3, new QStandardItem(QString("Stage")));
    ModelRunningGames->setHorizontalHeaderItem(4, new QStandardItem(QString("Speed")));
    ModelRunningGames->setHorizontalHeaderItem(5, new QStandardItem(QString("Players connected")));

    ui->TableViewServers->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    ui->TableViewServers->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->TableViewGeneral->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    ui->TableViewGeneral->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->StepTimeValue->setValidator(new QRegExpValidator(QRegExp("(0\\.|[1-9]\\d*\\.?)\\d*"), this));
    ui->StepTimeValue->setValidator(new QRegExpValidator(QRegExp("(0\\.|[1-9]\\d*\\.?)\\d*"), this));
    ui->ServerSelectIP->setValidator(new QRegExpValidator(QRegExp("(\\d{1,3}\\.){3}\\d{1,3}"), this));
    ui->ServerSelectPort->setValidator(new QRegExpValidator(QRegExp("\\d{1,5}"), this));
    ui->EditSentriesCount->setValidator(new QRegExpValidator(QRegExp("\\d{1,2}"), this));

    ui->playerLabel_1->setText("Player 1");
    ui->playerLabel_1->setStyleSheet("QLabel { background-color : blue; color : white;}");
    ui->playerLabel_2->setText("Player 2");
    ui->playerLabel_2->setStyleSheet("QLabel { background-color : orange; color : white;}");
    ui->playerLabel_3->setText("Player 3");
    ui->playerLabel_3->setStyleSheet("QLabel { background-color : green; color : white}");
    ui->playerLabel_4->setText("Player 4");
    ui->playerLabel_4->setStyleSheet("QLabel { background-color : red; color : white}");

    // Add shortcuts
    ui->PageGame->addAction(ui->actionGameEsc);
    ui->PageGame->addAction(ui->actionCenterPlayer);
    ui->GameView->addAction(ui->actionGoLeft);
    ui->GameView->addAction(ui->actionGoRight);
    ui->GameView->addAction(ui->actionGoUp);
    ui->GameView->addAction(ui->actionGoDown);
    ui->GameView->addAction(ui->actionGameAction);
    ui->PageServerSelect->addAction(ui->actionMenuEsc);
    ui->PageTableView->addAction(ui->actionMenuEsc);
    ui->PageLobby->addAction(ui->actionMenuEsc);

    tileTextures[(int)LevelMap::Tile::Forest] = QPixmap(QString("../art/forest1.png"));
    tileTextures[(int)LevelMap::Tile::Water] = QPixmap(QString("../art/water2.png"));
    tileTextures[(int)LevelMap::Tile::Bridge] = QPixmap(QString("../art/bridge1.png"));
    tileTextures[(int)LevelMap::Tile::Grass] = QPixmap(QString("../art/grass1.png"));
    tileTextures[(int)LevelMap::Tile::Path] = QPixmap(QString("../art/dirt1.png"));
    tileTextures[(int)LevelMap::Tile::Finish] = QPixmap(QString("../art/finish.png"));
    plankTexture = QPixmap(QString("../art/plank1.png"));

    QPixmap playerTextureAll(QString("../art/modernguy.png"));

    for (int i = 0; i < maxPlayers; i++)
    {
        QPixmap curTex(playerTextureAll);
        switch(i)
        {
        case 0:
            changeColor(curTex, QColor(136, 136, 136), QColor(0, 0, 136));
            changeColor(curTex, QColor(204, 212, 219), QColor(0, 0, 200));
            changeColor(curTex, QColor(170, 170, 170), QColor(0, 0, 170));
            changeColor(curTex, QColor(255, 255, 255), QColor(0, 0, 255));
            break;
        case 1:
            changeColor(curTex, QColor(136, 136, 136), QColor(136, 60, 0));
            changeColor(curTex, QColor(204, 212, 219), QColor(200, 80, 0));
            changeColor(curTex, QColor(170, 170, 170), QColor(170, 70, 0));
            changeColor(curTex, QColor(255, 255, 255), QColor(255, 127, 0));
            break;
        case 2:
            changeColor(curTex, QColor(136, 136, 136), QColor(0, 136, 0));
            changeColor(curTex, QColor(204, 212, 219), QColor(0, 200, 0));
            changeColor(curTex, QColor(170, 170, 170), QColor(0, 170, 0));
            changeColor(curTex, QColor(255, 255, 255), QColor(0, 255, 0));
            break;
        case 3:
            changeColor(curTex, QColor(136, 136, 136), QColor(136, 0, 0));
            changeColor(curTex, QColor(204, 212, 219), QColor(200, 0, 0));
            changeColor(curTex, QColor(170, 170, 170), QColor(170, 0, 0));
            changeColor(curTex, QColor(255, 255, 255), QColor(255, 0, 0));
            break;
        }

        playerTexture[i][(int)Direction::Down] = curTex.copy(0, 0, playerTextureWidth, playerTextureHeight);
        playerTexture[i][(int)Direction::Left] = curTex.copy(0, playerTextureHeight, playerTextureWidth, playerTextureHeight);
        playerTexture[i][(int)Direction::Right] = curTex.copy(0, 2 * playerTextureHeight, playerTextureWidth, playerTextureHeight);
        playerTexture[i][(int)Direction::Up] = curTex.copy(0, 3 * playerTextureHeight, playerTextureWidth, playerTextureHeight);
    }
    QPixmap sentryTextureAll(QString("../art/death_scythe.png"));
    sentryTexture[(int)Direction::Down] = sentryTextureAll.copy(0, 0, sentryTextureWidth, sentryTextureHeight);
    sentryTexture[(int)Direction::Left] = sentryTextureAll.copy(0, sentryTextureHeight, sentryTextureWidth, sentryTextureHeight);
    sentryTexture[(int)Direction::Right] = sentryTextureAll.copy(0, 2 * sentryTextureHeight, sentryTextureWidth, sentryTextureHeight);
    sentryTexture[(int)Direction::Up] = sentryTextureAll.copy(0, 3 * sentryTextureHeight, sentryTextureWidth, sentryTextureHeight);

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(100);

    gameMenu = nullptr;
    gameHistory = nullptr;

    game = nullptr;
    tcpClient = nullptr;
}

MainWindow::~MainWindow()
{
    if (game != nullptr)
    {
        leaveGame();
    }

    delete ModelServerSelection; ModelServerSelection = nullptr;
    delete ModelLevelSelection; ModelLevelSelection = nullptr;
    delete ModelSavedGames; ModelSavedGames = nullptr;
    delete ModelRunningGames; ModelRunningGames = nullptr;

    timer->stop();
    delete timer; timer = nullptr;

    if (tcpClient != nullptr)
    {
        tcpClient->stop();
        delete tcpClient; tcpClient = nullptr;
    }
}

void MainWindow::leaveGame()
{
    changePage(ui->PageMainMenu, false);

    delete game; game = nullptr;
    delete gameScene; gameScene = nullptr;
    for (int i = 0; i < maxPlayers; i++)
    {
        delete playerLabels[i]; playerLabels[i] = nullptr;
    }

    delete gameMenu; gameMenu = nullptr;
    delete gameHistory; gameHistory = nullptr;

    PacketPtr packet = PacketPtr(new Packet(CMSG_PLAYER_LEFT_GAME, 0));
    try { tcpClient->send(packet); }
    catch (...) { handleServerDisconnected(); return; }
}

void MainWindow::on_ButtonConnect_clicked()
{
    if (tcpClient == nullptr)
    {
        displayMsg("No server selected, please select one before continuing.");
        return;
    }

    changePage(ui->PageTableView, true, ModelRunningGames);
}

void MainWindow::on_ButtonLoadGame_clicked()
{
    if (tcpClient == nullptr)
    {
        displayMsg("No server selected, please select one before continuing.");
        return;
    }

    changePage(ui->PageTableView, true, ModelSavedGames);
}

void MainWindow::on_ButtonNewGame_clicked()
{
    if (tcpClient == nullptr)
    {
        displayMsg("No server selected, please select one before continuing.");
        return;
    }

    changePage(ui->PageTableView, true, ModelLevelSelection);
}

void MainWindow::changePage(QWidget *newPage, bool putInChain, void *object)
{
    if (putInChain)
        pages.push_back(ui->MainView->currentIndex());

    loadPage(newPage, object);
}

void MainWindow::previousPage()
{
    loadPage(ui->MainView->widget(pages.back()));
    pages.pop_back();
}

void MainWindow::loadPage(QWidget *page, void* object)
{
    ui->MainView->currentWidget()->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    if (page == ui->PageMainMenu)
    {
        pages.clear();
    }
    else if (page == ui->PageServerSelect)
    {
        ModelServerSelection->setRowCount(0);
        //TODO: refresh
        /*ModelServerSelection->appendRow(QList<QStandardItem*>({new QStandardItem(QString("127.0.0.1")),
                                                               new QStandardItem(QString("Great local server")),
                                                               new QStandardItem(QString("0"))}));*/
    }
    else if (page == ui->PageTableView)
    {
        try { loadTable((QStandardItemModel*)object); }
        catch (...) { handleServerDisconnected(); return; }
    }
    else if (page == ui->PageLobby)
    {

    }
    else if (page == ui->PageGameLoading)
    {

    }
    else if (page == ui->PageGame)
    {
        // Clear page history
        pages.clear();
        loadGamePage();
    }

    page->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    ui->MainView->setCurrentWidget(page);
}

void MainWindow::loadTable(QStandardItemModel *table)
{
    table->setRowCount(0);
    tableItemIds.clear();

    if (table == ModelLevelSelection)
    {
        tcpClient->send(PacketPtr(new Packet(CMSG_MAP_LIST_REQUEST, 0)));
        ui->TableViewTitle->setText(QString("Level selection"));
    }
    else if (table == ModelSavedGames)
    {
        table->appendRow(QList<QStandardItem*>({new QStandardItem(QDateTime::currentDateTime().toString("d.M.yyyy\thh:mm:ss")),
                                                new QStandardItem(QString("Awesome save")),
                                                new QStandardItem(QString("Such level")),
                                                new QStandardItem(QString("25x25")),
                                                new QStandardItem("5.0")}));
        ui->TableViewTitle->setText(QString("Saved games"));
    }

    else if (table == ModelRunningGames)
    {
        tcpClient->send(PacketPtr(new Packet(CMSG_GAME_LIST_REQUEST, 0)));
        ui->TableViewTitle->setText(QString("Running games"));
    }

    ui->TableViewGeneral->setModel(table);
}

void MainWindow::loadGame(uint8_t playerId, const std::string &mapData)
{
    game = new ClientGame();
    game->loadMap(mapData);
    myPlayerId = playerId;
    totalCommands = 0;
    failedCommands = 0;
}

void MainWindow::loadGamePage()
{
    const LevelMap &map = game->getMap();
    gameScene = new QGraphicsScene(ui->GameView);
    gameScene->setSceneRect(0, 0,
                            map.getWidth() * tileTextureSize + 2 * margin,
                            map.getHeight() * tileTextureSize + 2 * margin);


    playerLabels[0] = new PlayerLabel(ui->playerLabel_1, game, 0);
    playerLabels[1] = new PlayerLabel(ui->playerLabel_2, game, 1);
    playerLabels[2] = new PlayerLabel(ui->playerLabel_3, game, 2);
    playerLabels[3] = new PlayerLabel(ui->playerLabel_4, game, 3);

    ui->LabelGameMsg->setText("");

    gameMenu = new DialogGameMenu(this);
    gameHistory = new DialogGameHistory(this);

    ui->LabelGameMsg->setStyleSheet("QLabel { background-color : white; color : grey;}");

    ui->GameView->setScene(gameScene);
}

void MainWindow::setGameMsg(QString msg)
{
    lastGameMsgTime = std::chrono::high_resolution_clock::now();
    ui->LabelGameMsg->setText(msg);
    gameHistory->addToHistory(msg);
}

void MainWindow::displayMsg(QString msg)
{
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.setModal(true);
    msgBox.exec();
}

void MainWindow::sendCommand()
{
    if (!game->isRunning())
        return;

    ui->LabelCommandStatus->setText("Waiting");
    ui->LabelCommandStatus->setStyleSheet("QLabel { background-color : orange; color : white;}");
    QString commandText = ui->CommandInput->text();

    try
    {
        totalCommands++;

        PacketPtr packet = PacketPtr(new Packet(CMSG_PERFORM_ACTION_REQUEST, 1));

        if (commandText == "left")         *packet << (uint8_t)PLAYER_ACTION_ROTATE_LEFT;
        else if (commandText == "right")   *packet << (uint8_t)PLAYER_ACTION_ROTATE_RIGHT;
        else if (commandText == "up")      *packet << (uint8_t)PLAYER_ACTION_ROTATE_UP;
        else if (commandText == "down")    *packet << (uint8_t)PLAYER_ACTION_ROTATE_DOWN;
        else if (commandText == "go")      *packet << (uint8_t)PLAYER_ACTION_GO;
        else if (commandText == "action")  *packet << (uint8_t)PLAYER_ACTION_ACTION;
        else if (commandText == "stop")    *packet << (uint8_t)PLAYER_ACTION_STOP;
        else if (commandText == "take")    *packet << (uint8_t)PLAYER_ACTION_TAKE;
        else if (commandText == "open")    *packet << (uint8_t)PLAYER_ACTION_OPEN;
        else
        {
            gameHistory->addToHistory("Invalid command inputted.");
            ui->LabelCommandStatus->setText("Failed");
            ui->LabelCommandStatus->setStyleSheet("QLabel { background-color : red; color : white; font-weight:bold;}");
            failedCommands++;
            return;
        }

        gameHistory->addToHistory("Sending command: " + commandText);
        tcpClient->send(packet);
    }
    catch(...)
    {
        handleServerDisconnected(); return;
    }
}

void MainWindow::handleServerDisconnected()
{
    if (game != nullptr)
        leaveGame();

    tcpClient->stop();
    delete tcpClient; tcpClient = nullptr;
    ui->LabelServerIP->setText("No server selected");

    changePage(ui->PageMainMenu, false);
    displayMsg("Server closed unexpectedly.");
}

void MainWindow::update()
{
    if (tcpClient == nullptr)
        return;

    std::lock_guard<std::mutex> lock(tcpClientMutex);

    // Read server message
    PacketPtr response;

    try { response = tcpClient->getReceivedPacket(); }
    catch (...) { handleServerDisconnected(); return; }

    // Process server message, repeat while there are messages
    while (response)
    {
        if (ui->MainView->currentWidget() == ui->PageTableView)
        {
            if (ui->TableViewGeneral->model() == ModelRunningGames)
            {
                if (response->getOpcode() == SMSG_GAME_LIST_RESPONSE)
                {
                    uint32_t gamesCount;
                    *response >> gamesCount;
                    for (uint32_t i = 0; i < gamesCount; ++i)
                    {
                        uint32_t gameId;
                        std::string gameName;
                        uint8_t playerCount;
                        uint16_t stepTime;
                        std::string mapName;
                        uint8_t width, height;

                        *response >> gameId >> gameName >> playerCount >> stepTime >> mapName >> width >> height;

                        if (playerCount < maxPlayers)
                        {
                            tableItemIds.push_back(gameId);
                            ModelRunningGames->appendRow(QList<QStandardItem*>({
                                new QStandardItem(QString::fromStdString(gameName)),
                                new QStandardItem(QString::fromStdString(mapName)),
                                new QStandardItem(QString::number(width) + "x" + QString::number(height)),
                                new QStandardItem(QString("Very running.")),
                                new QStandardItem(QString::number((float)stepTime / 1000)),
                                new QStandardItem(QString::number(playerCount))
                            }));
                        }
                    }
                }
            }
            else if (ui->TableViewGeneral->model() == ModelLevelSelection)
            {
                if(response->getOpcode() == SMSG_MAP_LIST_RESPONSE)
                {
                    uint32_t mapsCount;
                    *response >> mapsCount;
                    for (uint32_t i = 0; i < mapsCount; ++i)
                    {
                        uint32_t mapId;
                        std::string mapName;
                        uint8_t width, height;
                        *response >> mapId >> mapName >> width >> height;

                        tableItemIds.push_back(mapId);
                        ModelLevelSelection->appendRow(QList<QStandardItem*>({
                            new QStandardItem(QString::fromStdString(mapName)),
                            new QStandardItem(QString::number(width) + "x" + QString::number(height))
                        }));
                    }
                }
            }
            else if (ui->TableViewGeneral->model() == ModelSavedGames)
            {
                // Display games saved on server
            }
        }
        else if(ui->MainView->currentWidget() == ui->PageGameLoading)
        {
            if (response->getOpcode() == SMSG_GAME_CREATE_RESPONSE)
            {
                bool success;
                uint8_t playerId;
                std::string mapData;
                *response >> success;
                if (success)
                {
                    *response >> playerId >> mapData;
                    loadGame(playerId, mapData);
                    changePage(ui->PageGame, false);
                }
            }
            else if (response->getOpcode() == SMSG_GAME_JOIN_RESPONSE)
            {
                bool canJoin;
                uint8_t playerId;
                std::string mapData;
                *response >> canJoin;
                if (canJoin)
                {
                    *response >> playerId >> mapData;
                    loadGame(playerId, mapData);
                    changePage(ui->PageGame, false);
                }
            }
        }
        else if (ui->MainView->currentWidget() == ui->PageGame && game->isRunning())
        {
            if (response->getOpcode() == SMSG_GAME_END)
            {
                redrawScene(); // last scene redraw
                uint8_t winPlayerId;
                *response >> winPlayerId;

                game->end();

                if (winPlayerId == myPlayerId)
                {
                    ui->LabelGameMsg->setStyleSheet("QLabel { background-color : green; color : white;}");
                    setGameMsg("You've won, congratulations!");
                }
                else
                {
                    ui->LabelGameMsg->setStyleSheet("QLabel { background-color : red; color : white;}");
                    setGameMsg("You've lost, better luck next time");
                }
            }
            else if (response->getOpcode() == SMSG_GAME_CREATE_OBJECT)
            {
                uint32_t count;
                *response >> count;

                uint8_t objType;
                uint8_t posX, posY;
                uint8_t rotation;
                uint8_t objId;

                for (uint32_t i = 0; i < count; i++)
                {
                    *response >> objType >> posX >> posY;
                    if (objType == OBJECT_TYPE_PLAYER)
                    {
                        bool alive;
                        uint32_t deaths;
                        *response >> rotation >> objId >> alive >> deaths;
                        game->addPlayer(objId);
                        game->movePlayer(objId, Position(posX, posY), (Direction)rotation);
                        game->setPlayerState(objId, alive, deaths);
                        if (objId == myPlayerId)
                            setGameMsg("You have connected into game.");
                        else
                            setGameMsg("Player " + QString::number(objId + 1) + " connected into game.");
                    }
                    else if (objType == OBJECT_TYPE_SENTRY)
                    {
                        *response >> rotation >> objId;
                        game->addSentry(objId);
                        game->moveSentry(objId, Position(posX, posY), (Direction)rotation);
                    }
                    else if (objType == OBJECT_TYPE_PLANK)
                    {
                        game->placePlank(Position(posX, posY));
                        setGameMsg("Plank has been dropped.");
                    }
                    else if (objType == OBJECT_TYPE_BRIDGE)
                    {
                        game->placeBridge(Position(posX, posY));
                        setGameMsg("Bridge has been built.");
                    }
                }
            }
            else if (response->getOpcode() == SMSG_GAME_UPDATE_OBJECT)
            {
                uint32_t count;
                *response >> count;

                uint8_t objType;
                uint8_t posX, posY;
                uint8_t rotation;
                uint8_t objId;

                for (uint32_t i = 0; i < count; i++)
                {
                    *response >> objType >> objId >> posX >> posY >> rotation;
                    if (objType == OBJECT_TYPE_PLAYER)
                    {
                        bool alive, wasAlive;
                        uint32_t deaths;
                        *response >> alive >> deaths;
                        game->movePlayer(objId, Position(posX, posY), (Direction)rotation);
                        wasAlive = game->getPlayer(objId)->isAlive();
                        if (alive != wasAlive)
                        {
                            if (myPlayerId == objId)
                            {
                                if (alive)
                                    setGameMsg("You have respawned.");
                                else
                                    setGameMsg("You have been killed.");
                            }
                            else
                            {
                                if (alive)
                                    setGameMsg("Player " + QString::number(objId + 1) + " has respawned,");
                                else
                                    setGameMsg("Player " + QString::number(objId + 1) + " has been killed.");
                            }
                        }
                        game->setPlayerState(objId, alive, deaths);
                    }
                    else if (objType == OBJECT_TYPE_SENTRY)
                    {
                        game->moveSentry(objId, Position(posX, posY), (Direction)rotation);
                    }
                }
            }
            else if (response->getOpcode() == SMSG_GAME_DELETE_OBJECT)
            {
                uint32_t count;
                *response >> count;

                uint8_t objType;
                uint8_t objId;

                for (uint32_t i = 0; i < count; i++)
                {
                    *response >> objType;
                    if (objType == OBJECT_TYPE_PLAYER)
                    {
                        *response >> objId;
                        game->removePlayer(objId);
                        setGameMsg("Player " + QString::number(objId) + " has disconnected.");
                    }
                    else if (objType == OBJECT_TYPE_PLANK)
                    {
                        game->removePlank();
                        setGameMsg("Plank has been picked up.");
                    }
                }
            }
            else if (response->getOpcode() == SMSG_PERFORM_ACTION_RESPONSE)
            {
                bool success;
                *response >> success;

                if (success)
                {
                    gameHistory->addToHistory("Command successful.");
                    ui->LabelCommandStatus->setText("O.K.");
                    ui->LabelCommandStatus->setStyleSheet("QLabel { background-color : green; color : white;}");
                }
                else
                {
                    gameHistory->addToHistory("Failed to execute command.");
                    ui->LabelCommandStatus->setText("Failed");
                    ui->LabelCommandStatus->setStyleSheet("QLabel { background-color : red; color : white; font-weight:bold;}");
                    failedCommands++;
                }
            }
        }

        try { response = tcpClient->getReceivedPacket(); }
        catch (...) { handleServerDisconnected(); return; }
    }

    if (ui->MainView->currentWidget() == ui->PageGame && game->isRunning())
    {
        // Clear game message if too old
        if (ui->LabelGameMsg->text() != "")
        {
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto dur = std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastGameMsgTime).count();
            if (dur > 3)
                ui->LabelGameMsg->setText("");
        }

        // Refresh commands count
        ui->LabelTotalCommands->setText(QString::number(totalCommands));
        ui->LabelFailedCommands->setText(QString::number(failedCommands));

        // Redraw scene and update game time
        redrawScene();
        ui->LabelGameTime->setText(formatTime(game->getTime()));
    }
}

QPoint MainWindow::tileToPoint(int x, int y, double xc, double yc)
{
    double px = margin + x * tileTextureSize + xc / 2.0;
    double py = margin + y * tileTextureSize + yc / 2.0;
    return QPoint(px, py);
}

void MainWindow::redrawScene()
{
    gameScene->clear();

    QGraphicsPixmapItem *item = nullptr;
    const LevelMap &map = game->getMap();
    int rows = map.getHeight();
    int columns = map.getWidth();
    LevelMap::Tile tile;

    // Draw tiles
    for (int dx = 0; dx < columns; dx++)
    {
        for (int dy = 0; dy < rows; dy++)
        {
            Position pos = Position(dx, dy);
            item = new QGraphicsPixmapItem();
            item->setPos(tileToPoint(dx, dy));

            tile = map.getTileAt(pos);
            if (tile == LevelMap::Tile::Finish || tile == LevelMap::Tile::Bridge)
            {
                if (tile == LevelMap::Tile::Finish)
                    item->setPixmap(tileTextures[(int)LevelMap::Tile::Path]);
                else
                    item->setPixmap(tileTextures[(int)LevelMap::Tile::Water]);
                gameScene->addItem(item);
                item = new QGraphicsPixmapItem();
                item->setPos(tileToPoint(dx, dy));
            }

            item->setPixmap(tileTextures[(int)tile]);
            gameScene->addItem(item);
        }
    }

    // Draw plank
    if (game->isPlankDropped())
    {
        item = new QGraphicsPixmapItem();
        item->setPixmap(plankTexture);
        Position pos = game->getPlankPos();
        item->setPos(tileToPoint(pos.x, pos.y));
        gameScene->addItem(item);
    }

    // Draw players
    const std::list<Player> &players = game->getPlayers();
    for (auto it = players.begin(); it != players.end(); it++)
    {
        if (!it->isAlive())
            continue;

        item = new QGraphicsPixmapItem();
        item->setPixmap(playerTexture[it->getId()][(int)it->getDirection()]);
        Position pos = it->getPosition();
        item->setPos(tileToPoint(pos.x, pos.y, tileTextureSize - playerTextureWidth, tileTextureSize - playerTextureHeight));
        gameScene->addItem(item);
    }

    // Draw sentries
    const std::list<Sentry> &sentries = game->getSentries();
    for (auto it = sentries.begin(); it != sentries.end(); it++)
    {
        item = new QGraphicsPixmapItem();
        item->setPixmap(sentryTexture[(int)it->getDirection()]);
        Position pos = it->getPosition();
        item->setPos(tileToPoint(pos.x, pos.y, tileTextureSize - sentryTextureWidth, tileTextureSize - sentryTextureHeight));
        gameScene->addItem(item);
    }
}

void MainWindow::changeColor(QPixmap& pixmap, QColor origColor, QColor newColor)
{
    QBitmap mask(pixmap.createMaskFromColor(origColor, Qt::MaskOutColor));

    QPainter paint(&pixmap);
    paint.setPen(newColor);
    paint.drawPixmap(pixmap.rect(), mask, mask.rect());
    paint.end();
}

void MainWindow::checkGameJoined()
{
    if (ui->MainView->currentWidget() == ui->PageGameLoading)
    {
        displayMsg("Failed to join game.");
        changePage(ui->PageTableView, false, ModelRunningGames);
    }
}

void MainWindow::on_TableViewGeneral_doubleClicked(const QModelIndex &index)
{
    if (ui->TableViewGeneral->model() == ModelLevelSelection)
    {
        selectedLevelId = tableItemIds.at(index.row());
        changePage(ui->PageLobby, false);
    }
    else if (ui->TableViewGeneral->model() == ModelSavedGames)
    {
        //TODO load saved game
        changePage(ui->PageLobby, false);
    }
    else if (ui->TableViewGeneral->model() == ModelRunningGames)
    {
        if (true)
        {
            // Connect directly to running game
            PacketPtr packet = PacketPtr(new Packet(CMSG_GAME_JOIN_REQUEST, 4));
            *packet << tableItemIds.at(index.row());

            changePage(ui->PageGameLoading, false);

            try { tcpClient->send(packet); }
            catch (...) { handleServerDisconnected(); return; }

            QTimer::singleShot(3000, this, SLOT(checkGameJoined()));
        }
        else
        {
            //TODO connect to lobby
            changePage(ui->PageLobby, false);
        }
    }
}

void MainWindow::on_ButtonTableViewBack_clicked()
{
    previousPage();
}

void MainWindow::on_ButtonLobbyBack_clicked()
{
    previousPage();
}

void MainWindow::on_ButtonLobbyLevelSelect_clicked()
{
    changePage(ui->PageTableView, false, ModelLevelSelection);
}

void MainWindow::on_StepTimeValue_editingFinished()
{
    QLocale c(QLocale::C);
    double speed = c.toDouble(ui->StepTimeValue->text());

    if (speed > 5.0)
    {
        speed = 5.0;
        ui->StepTimeValue->setText(c.toString(speed));
    }
    else if (speed < 0.5)
    {
        speed = 0.5;
        ui->StepTimeValue->setText(c.toString(speed));
    }
}

PlayerLabel::PlayerLabel(QObject *parent, const Game * game, int id)
{
    parent->installEventFilter(this);

    label = (QLabel *)parent;
    _game = game;
    _id = id;
}

bool PlayerLabel::eventFilter(QObject *object, QEvent *event)
{
    if (object == label)
    {
        if (event->type() == QEvent::Enter)
        {
            // Get stats for player with ID of this label
            const Player *labelPlayer = _game->getPlayer(_id);
            QString text("Player " + QString::number(_id + 1) + " statistics:\n");
            if (labelPlayer == nullptr)
                text.append("Not connected");
            else
            {
                int gameTime = labelPlayer->getJoinTime() - _game->getTime();
                text.append("Game time: " + formatTime(gameTime) + "\n");
                text.append(QString("Alive: ") + (labelPlayer->isAlive() ? "Yes" : "No") + "\n");
                text.append("Deaths: " + QString::number(labelPlayer->getDeaths()));
            }
            QToolTip::showText(label->mapToGlobal(QPoint( 0, 10 ) ), text);
            return true;
        }
        else if (event->type() == QEvent::Leave)
        {
            QToolTip::hideText();
            return true;
        }
    }
    return false;
}

void MainWindow::on_CommandInput_returnPressed()
{
    sendCommand();
}

void MainWindow::on_ButtonSendCommand_clicked()
{
    sendCommand();
}

void MainWindow::on_actionGameEsc_triggered()
{
    gameMenu->show();
}

void MainWindow::on_actionMenuEsc_triggered()
{
    previousPage();
}

void MainWindow::on_ButtonServerManual_clicked()
{
    timer->stop();

    QString address = ui->ServerSelectIP->text();
    int port = ui->ServerSelectPort->text().toInt();

    int pos = 0;
    if(((const QRegExpValidator*)ui->ServerSelectIP->validator())->validate(address, pos) !=
            QValidator::State::Acceptable)
    {
        displayMsg("Incorrect server IP.");
        return;
    }

    if (port < 10000 || port > 65535)
    {
        displayMsg("Server port must be number between 10000 and 65535.");
        return;
    }

    TcpClient *tmpClient = new TcpClient(address.toStdString(), port);
    try
    {
        tmpClient->start();

        // Handshake
        PacketPtr packet = PacketPtr(new Packet(CMSG_HANDSHAKE_REQUEST, 4));
        *packet << (uint32_t)1337;
        tmpClient->send(packet);

        // Wait second for response
        PacketPtr response = tmpClient->getReceivedPacket(1000);

        if (!response || response->getOpcode() != SMSG_HANDSHAKE_RESPONSE)
            throw MsgException("Server unavailable.");
    }
    catch(...)
    {
        displayMsg("Unable to connect to server.");
        tmpClient->stop();
        delete tmpClient; tmpClient = nullptr;
        return;
    }

    if (tcpClient != nullptr)
    {
        tcpClient->stop();
        delete tcpClient; tcpClient = nullptr;
    }

    tcpClient = tmpClient;
    ui->LabelServerIP->setText(address);
    previousPage();
    timer->start();
}

void MainWindow::on_actionCenterPlayer_triggered()
{
    // Focus on player
    const Player *myPlayer = game->getPlayer(myPlayerId);
    ui->GameView->centerOn(tileToPoint(myPlayer->getPosition().x, myPlayer->getPosition().y));
}


void MainWindow::on_actionGoLeft_triggered()
{
    ui->CommandInput->setText("left");
    sendCommand();
}

void MainWindow::on_actionGoUp_triggered()
{
    ui->CommandInput->setText("up");
    sendCommand();
}

void MainWindow::on_actionGoRight_triggered()
{
    ui->CommandInput->setText("right");
    sendCommand();
}

void MainWindow::on_actionGoDown_triggered()
{
    ui->CommandInput->setText("down");
    sendCommand();
}

void MainWindow::on_actionGameAction_triggered()
{
    ui->CommandInput->setText("action");
    sendCommand();
}

void MainWindow::on_ButtonServerRefresh_clicked()
{
    // Fetch all servers

}

void MainWindow::on_ButtonServerBack_clicked()
{
    previousPage();
}

void MainWindow::on_ButtonChangeServer_clicked()
{
    changePage(ui->PageServerSelect);
}

void MainWindow::on_TableViewServers_doubleClicked(const QModelIndex &index)
{
    //TODO: get ip from data loaded
    ui->LabelServerIP->setText("127.0.0.1");
    previousPage();
}

void MainWindow::checkGameCreated()
{
    if (ui->MainView->currentWidget() == ui->PageGameLoading)
    {
        displayMsg("Failed to create new game.");
        changePage(ui->PageLobby, false);
    }
}

void MainWindow::on_ButtonStartGame_clicked()
{
    std::string gameName = ui->EditGameName->text().toStdString();
    PacketPtr packet = PacketPtr(new Packet(CMSG_GAME_CREATE_REQUEST, 4 + gameName.length() + 1 + 2 + 1));
    uint16_t stepTime = (uint16_t)(ui->StepTimeValue->text().toFloat() * 1000);
    uint8_t sentriesCount = (uint8_t)(ui->EditSentriesCount->text().toInt());
    *packet << selectedLevelId << gameName << stepTime << sentriesCount;

    changePage(ui->PageGameLoading, false);

    try { tcpClient->send(packet); }
    catch (...) { handleServerDisconnected(); return; }

    QTimer::singleShot(3000, this, SLOT(checkGameCreated()));
}

void MainWindow::on_ButtonCommandHistory_clicked()
{
    gameHistory->exec();
}
