#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialoggamemenu.h"
#include <QDateTime>
#include <QHeaderView>
#include <QGraphicsPixmapItem>
#include <QScrollBar>

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
    QPixmap pixmap(QString("forest.png"));
    QGraphicsPixmapItem *item = nullptr;

    int rows = 50;
    int columns = 50;
    double margin = 20;
    double w = pixmap.width();
    double h = pixmap.height();

    gameScene = new QGraphicsScene(ui->GameView);
    gameScene->setSceneRect(0, 0, columns * w + 2 * margin, rows * h + 2 * margin);
    ui->GameView->setScene(gameScene);

    for (int dx = 0; dx < columns; dx++)
    {
        for (int dy = 0; dy < rows; dy++)
        {
            item = new QGraphicsPixmapItem();
            item->setPixmap(pixmap);
            item->setPos(margin + dx * w, margin + dy * h);
            gameScene->addItem(item);
        }
    }
    ui->GameView->centerOn(100, 100);
}

void MainWindow::sendCommand()
{
    //TODO send this
    ui->CommandInput->text();
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
