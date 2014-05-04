#include "gui/mainwindow.h"
#include "ui_mainwindow.h"
#include "gui/dialoggamemenu.h"
#include <QDateTime>
#include <QHeaderView>
#include <QScrollBar>

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

    ui->playerLabel_1->setStyleSheet("QLabel { background-color : red; color : blue; }");

    playerLabels[0] = new PlayerLabel(ui->playerLabel_1);
    playerLabels[1] = new PlayerLabel(ui->playerLabel_2);
    playerLabels[2] = new PlayerLabel(ui->playerLabel_3);
    playerLabels[3] = new PlayerLabel(ui->playerLabel_4);

    gameMenu = new DialogGameMenu(this);
    ui->PageGame->addAction(ui->actionGameEsc);

    ui->PageTableView->addAction(ui->actionMenuEsc);
    ui->PageLobby->addAction(ui->actionMenuEsc);

    tileTextures[(int)LevelMap::Tile::Forest] = QPixmap(QString("../art/forest1.png"));
    tileTextures[(int)LevelMap::Tile::Water] = QPixmap(QString("../art/water2.png"));
    tileTextures[(int)LevelMap::Tile::Bridge] = QPixmap(QString("../art/bridge1.png"));
    tileTextures[(int)LevelMap::Tile::Grass] = QPixmap(QString("../art/dirt1.png"));
    tileTextures[(int)LevelMap::Tile::Path] = QPixmap(QString("../art/dirt1.png"));
    plankTexture = QPixmap(QString("../art/plank1.png"));

    QPixmap playerTextureAll(QString("../art/brian.png"));
    playerTexture[(int)Direction::Down] = playerTextureAll.copy(0, 0, playerTextureWidth, playerTextureHeight);
    playerTexture[(int)Direction::Left] = playerTextureAll.copy(0, playerTextureHeight, playerTextureWidth, playerTextureHeight);
    playerTexture[(int)Direction::Right] = playerTextureAll.copy(0, 2 * playerTextureHeight, playerTextureWidth, playerTextureHeight);
    playerTexture[(int)Direction::Up] = playerTextureAll.copy(0, 3 * playerTextureHeight, playerTextureWidth, playerTextureHeight);

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

    ui->GameView->setScene(gameScene);
}

void MainWindow::sendCommand()
{
    //TODO send this
    ui->CommandInput->text();
}

void MainWindow::update()
{
    //TODO: Process server messages

    // If in-game, redraw scene
    if (ui->MainView->currentWidget() == ui->PageGame)
        redrawScene();
}

void MainWindow::redrawScene()
{
    // Focus on player
    ui->GameView->centerOn(100, 100);

    static int i = 0;
    gameScene->clear();

    QGraphicsPixmapItem *item = nullptr;
    const LevelMap &map = game.getMap();
    int rows = map.getHeight();
    int columns = map.getWidth();
    LevelMap::Tile tile;

    for (int dx = 0; dx < columns; dx++)
    {
        for (int dy = 0; dy < rows; dy++)
        {
            Position pos = Position(dx, dy);
            item = new QGraphicsPixmapItem();
            tile = map.getTileAt(pos);
            item->setPixmap(tileTextures[(int)tile]);
            item->pixmap().scaled(tileTextureSize, tileTextureSize, Qt::KeepAspectRatio);
            item->setPos(margin + dx * tileTextureSize, margin + dy * tileTextureSize);
            gameScene->addItem(item);
        }
    }

    const std::list<Player> &players = game.getPlayers();
    for (auto it = players.begin(); it != players.end(); it++)
    {
        item = new QGraphicsPixmapItem();
        item->setPixmap(playerTexture[(int)it->getDirection()]);
        Position pos = it->getPosition();
        item->setPos(margin + i + pos.x * tileTextureSize, margin + pos.y * tileTextureSize);
        gameScene->addItem(item);
    }
    i++;
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

PlayerLabel::PlayerLabel(QObject *parent)
{
    parent->installEventFilter(this);

    label = (QLabel *)parent;
}

bool PlayerLabel::eventFilter(QObject *object, QEvent *event)
{
    if (object == label)
    {
        if (event->type() == QEvent::Enter)
        {
            label->setText("Howering");
            return true;
        }
        else if (event->type() == QEvent::Leave)
        {
            label->setText("Not howering");
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
