#include "gui/mainwindow.h"
#include "ui_mainwindow.h"
#include "gui/dialoggamemenu.h"
#include <QDateTime>
#include <QHeaderView>
#include <QScrollBar>
#include <QToolTip>
#include <QBitmap>
#include <QPainter>

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
    ui(new Ui::MainWindow),
    game()
{
    ui->setupUi(this);

    ui->GameView->horizontalScrollBar()->setStyleSheet("QScrollBar {height:0px;}");
    ui->GameView->horizontalScrollBar()->hide();
    ui->GameView->verticalScrollBar()->setStyleSheet("QScrollBar {width:0px;}");
    ui->GameView->verticalScrollBar()->hide();

    loadPage(ui->PageMainMenu);
    gameScene = nullptr;

    ModelLevelSelection = new QStandardItemModel(0, 2, this);
    ModelLevelSelection->setHorizontalHeaderItem(0, new QStandardItem(QString("Name")));
    ModelLevelSelection->setHorizontalHeaderItem(1, new QStandardItem(QString("Size")));

    ModelSavedGames = new QStandardItemModel(0, 5, this);
    ModelSavedGames->setHorizontalHeaderItem(0, new QStandardItem(QString("Date")));
    ModelSavedGames->setHorizontalHeaderItem(1, new QStandardItem(QString("Name")));
    ModelSavedGames->setHorizontalHeaderItem(2, new QStandardItem(QString("Level")));
    ModelSavedGames->setHorizontalHeaderItem(3, new QStandardItem(QString("Size")));
    ModelSavedGames->setHorizontalHeaderItem(4, new QStandardItem(QString("Speed")));

    ModelRunningGames = new QStandardItemModel(0, 4, this);
    ModelRunningGames->setHorizontalHeaderItem(0, new QStandardItem(QString("Host")));
    ModelRunningGames->setHorizontalHeaderItem(1, new QStandardItem(QString("Name")));
    ModelRunningGames->setHorizontalHeaderItem(2, new QStandardItem(QString("Size")));
    ModelRunningGames->setHorizontalHeaderItem(3, new QStandardItem(QString("Stage")));

    ui->TableViewGeneral->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);

    ui->StepTimeValue->setValidator(new QRegExpValidator(QRegExp("(0\\.|[1-9]\\d*\\.?)\\d*")));
    ui->CommandInput->setValidator(new QRegExpValidator(QRegExp("[a-z]*")));

    ui->playerLabel_1->setText("Player 1");
    ui->playerLabel_1->setStyleSheet("QLabel { background-color : blue; color : white;}");
    ui->playerLabel_2->setText("Player 2");
    ui->playerLabel_2->setStyleSheet("QLabel { background-color : orange; color : white;}");
    ui->playerLabel_3->setText("Player 3");
    ui->playerLabel_3->setStyleSheet("QLabel { background-color : green; color : white}");
    ui->playerLabel_4->setText("Player 4");
    ui->playerLabel_4->setStyleSheet("QLabel { background-color : red; color : white}");

    playerLabels[0] = new PlayerLabel(ui->playerLabel_1, &game, 0);
    playerLabels[1] = new PlayerLabel(ui->playerLabel_2, &game, 1);
    playerLabels[2] = new PlayerLabel(ui->playerLabel_3, &game, 2);
    playerLabels[3] = new PlayerLabel(ui->playerLabel_4, &game, 3);

    gameMenu = new DialogGameMenu(this);

    // Add shortcuts
    ui->PageGame->addAction(ui->actionGameEsc);
    ui->PageGame->addAction(ui->actionCenterPlayer);
    ui->GameView->addAction(ui->actionGoLeft);
    ui->GameView->addAction(ui->actionGoRight);
    ui->GameView->addAction(ui->actionGoUp);
    ui->GameView->addAction(ui->actionGoDown);
    ui->GameView->addAction(ui->actionGameAction);
    ui->PageTableView->addAction(ui->actionMenuEsc);
    ui->PageLobby->addAction(ui->actionMenuEsc);

    tileTextures[(int)LevelMap::Tile::Forest] = QPixmap(QString("../art/forest1.png"));
    tileTextures[(int)LevelMap::Tile::Water] = QPixmap(QString("../art/water2.png"));
    tileTextures[(int)LevelMap::Tile::Bridge] = QPixmap(QString("../art/bridge1.png"));
    tileTextures[(int)LevelMap::Tile::Grass] = QPixmap(QString("../art/dirt1.png"));
    tileTextures[(int)LevelMap::Tile::Path] = QPixmap(QString("../art/dirt1.png"));
    plankTexture = QPixmap(QString("../art/plank1.png"));

    QPixmap playerTextureAll(QString("../art/modernguy.png"));

    for (int i = 0; i < maxPlayers; i++)
    {
        QPixmap curTex(playerTextureAll);
        switch(i)
        {
        case 0:
            changeColor(curTex, QColor(204, 212, 219), QColor(0, 0, 255));
            changeColor(curTex, QColor(170, 170, 170), QColor(0, 0, 255));
            changeColor(curTex, QColor(255, 255, 255), QColor(0, 0, 255));
            break;
        case 1:
            changeColor(curTex, QColor(204, 212, 219), QColor(255, 127, 0));
            changeColor(curTex, QColor(170, 170, 170), QColor(255, 127, 0));
            changeColor(curTex, QColor(255, 255, 255), QColor(255, 127, 0));
            break;
        case 2:
            changeColor(curTex, QColor(204, 212, 219), QColor(0, 255, 0));
            changeColor(curTex, QColor(170, 170, 170), QColor(0, 255, 0));
            changeColor(curTex, QColor(255, 255, 255), QColor(0, 255, 0));
            break;
        case 3:
            changeColor(curTex, QColor(204, 212, 219), QColor(255, 0, 0));
            changeColor(curTex, QColor(170, 170, 170), QColor(255, 0, 0));
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


    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(100);
}

MainWindow::~MainWindow()
{
    delete ModelLevelSelection; ModelLevelSelection = nullptr;
    delete ModelSavedGames; ModelSavedGames = nullptr;
    delete ModelRunningGames; ModelRunningGames = nullptr;
    for (int i = 0; i < maxPlayers; i++)
    {
        delete playerLabels[i];
        playerLabels[i] = nullptr;
    }
    delete gameMenu; gameMenu = nullptr;
    delete ui; ui = nullptr;
    delete timer; timer = nullptr;
}

void MainWindow::leaveGame()
{
    delete gameScene; gameScene = nullptr;
    changePage(ui->PageMainMenu, false);
}

void MainWindow::on_ButtonConnect_clicked()
{
    changePage(ui->PageTableView, true, ModelRunningGames);
}

void MainWindow::on_ButtonLoadGame_clicked()
{
    changePage(ui->PageTableView, true, ModelSavedGames);
}

void MainWindow::on_ButtonNewGame_clicked()
{
    changePage(ui->PageTableView, true, ModelLevelSelection);
}

void MainWindow::changePage(QWidget *newPage, bool putInChain, void *object)
{
    if (putInChain)
        pages.push_back(ui->MainView->currentWidget());
    loadPage(newPage, object);
}

void MainWindow::previousPage()
{
    loadPage(pages.back());
    pages.pop_back();
}

void MainWindow::loadPage(QWidget *page, void* object)
{
    ui->MainView->currentWidget()->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    if (page == ui->PageMainMenu)
    {

    }
    else if (page == ui->PageTableView)
    {
        loadTable((QStandardItemModel*)object);
    }
    else if (page == ui->PageLobby)
    {

    }
    else if (page == ui->PageGame)
    {
        // Clear page history
        pages.clear();
        loadGame();
    }

    page->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    ui->MainView->setCurrentWidget(page);
}

void MainWindow::loadTable(QStandardItemModel *table)
{
    if (table == ModelLevelSelection)
    {
        table->appendRow(QList<QStandardItem*>({new QStandardItem(QString("Such level")), new QStandardItem(QString("25x25"))}));
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
        table->appendRow(QList<QStandardItem*>({new QStandardItem(QString("Doge")),
                                                new QStandardItem(QString("Such level")),
                                                new QStandardItem(QString("25x25")),
                                                new QStandardItem(QString("Very running."))}));

        table->appendRow(QList<QStandardItem*>({new QStandardItem(QString("Doge")),
                                                new QStandardItem(QString("Such level")),
                                                new QStandardItem(QString("25x25")),
                                                new QStandardItem(QString("So waiting."))}));
        ui->TableViewTitle->setText(QString("Running games"));
    }

    ui->TableViewGeneral->setModel(table);
}

void MainWindow::loadGame()
{
    const LevelMap &map = game.getMap();
    gameScene = new QGraphicsScene(ui->GameView);
    gameScene->setSceneRect(0, 0, map.getWidth() * tileTextureSize + 2 * margin, map.getHeight() * tileTextureSize + 2 * margin);

    myPlayerId = 0;
    ui->GameView->setScene(gameScene);
}

void MainWindow::sendCommand()
{
    ui->LabelCommandStatus->setText("Waiting");
    ui->LabelCommandStatus->setStyleSheet("QLabel { background-color : orange; color : white;}");

    if (ui->CommandInput->text() == "left")
    {

    }
    else if (ui->CommandInput->text() == "right")
    {

    }
    else if (ui->CommandInput->text() == "up")
    {

    }
    else if (ui->CommandInput->text() == "down")
    {

    }
    else if (ui->CommandInput->text() == "action")
    {

    }
    else
    {
        ui->LabelCommandStatus->setText("Failed");
        ui->LabelCommandStatus->setStyleSheet("QLabel { background-color : red; color : white; font-weight:bold;}");
        return;
    }

    //TODO send this
    ui->CommandInput->text();

    ui->LabelCommandStatus->setText("O.K.");
    ui->LabelCommandStatus->setStyleSheet("QLabel { background-color : green; color : white;}");
}

void MainWindow::update()
{
    //TODO: Process server messages

    // If in-game, redraw scene
    if (ui->MainView->currentWidget() == ui->PageGame)
        redrawScene();
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
    const LevelMap &map = game.getMap();
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
            tile = map.getTileAt(pos);
            item->setPixmap(tileTextures[(int)tile]);
            item->pixmap().scaled(tileTextureSize, tileTextureSize, Qt::KeepAspectRatio);          
            item->setPos(tileToPoint(dx, dy));
            gameScene->addItem(item);
        }
    }

    // Draw players
    const std::list<Player> &players = game.getPlayers();
    for (auto it = players.begin(); it != players.end(); it++)
    {
        item = new QGraphicsPixmapItem();
        item->setPixmap(playerTexture[it->getId()][(int)it->getDirection()]);
        Position pos = it->getPosition();
        item->setPos(tileToPoint(pos.x, pos.y, tileTextureSize - playerTextureWidth, tileTextureSize - playerTextureHeight));
        gameScene->addItem(item);
    }

    // Draw sentries
    const std::list<Sentry> &sentries = game.getSentries();
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

void MainWindow::on_TableViewGeneral_doubleClicked(const QModelIndex &index)
{
    if (ui->TableViewGeneral->model() == ModelLevelSelection)
    {
        //TODO load level as such
        changePage(ui->PageLobby, false);
    }
    else if (ui->TableViewGeneral->model() == ModelSavedGames)
    {
        //TODO load saved game
        changePage(ui->PageLobby, false);
    }
    else if (ui->TableViewGeneral->model() == ModelRunningGames)
    {
        if (index.row() % 2 == 0)
        {
            //TODO connect to running game
            changePage(ui->PageGame, false);
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
    changePage(ui->PageTableView, true, ModelLevelSelection);
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
                text.append("Kills: " + QString::number(labelPlayer->getKills()));
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
    // Find server at ui->ServerSelectIP and display it
}

void MainWindow::on_actionCenterPlayer_triggered()
{
    // Focus on player
    const Player *myPlayer = game.getPlayer(myPlayerId);
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
