/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "gui/zoomgraphicsview.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionGameEsc;
    QAction *actionMenuEsc;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QStackedWidget *MainView;
    QWidget *PageMainMenu;
    QGridLayout *gridLayout_2;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *ButtonNewGame;
    QPushButton *ButtonLoadGame;
    QPushButton *ButtonConnect;
    QSpacerItem *verticalSpacer_5;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *verticalSpacer_4;
    QWidget *PageTableView;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QLabel *TableViewTitle;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *ButtonTableViewBack;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_4;
    QTableView *TableViewGeneral;
    QWidget *PageLobby;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *ButtonLobbyBack;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer_9;
    QVBoxLayout *verticalLayout_6;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer_10;
    QPushButton *PlayerSlot_1;
    QSpacerItem *verticalSpacer_9;
    QPushButton *PlayerSlot_2;
    QSpacerItem *verticalSpacer_8;
    QPushButton *PlayerSlot_3;
    QSpacerItem *verticalSpacer_7;
    QPushButton *PlayerSlot_4;
    QSpacerItem *verticalSpacer_6;
    QHBoxLayout *horizontalLayout_5;
    QLineEdit *StepTimeValue;
    QLabel *StepTimeDescriptor;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *ButtonLobbyLevelSelect;
    QSpacerItem *horizontalSpacer_7;
    QCheckBox *CheckBoxLobbyClosed;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *pushButton_2;
    QWidget *PageGame;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_9;
    QLabel *playerLabel_1;
    QLabel *playerLabel_2;
    QLabel *playerLabel_3;
    QLabel *playerLabel_4;
    ZoomGraphicsView *GameView;
    QSpacerItem *verticalSpacer_11;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *CommandInput;
    QLabel *LabelCommandStatus;
    QPushButton *ButtonSendCommand;
    QPushButton *ButtonCommandHistory;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(942, 700);
        actionGameEsc = new QAction(MainWindow);
        actionGameEsc->setObjectName(QStringLiteral("actionGameEsc"));
        actionMenuEsc = new QAction(MainWindow);
        actionMenuEsc->setObjectName(QStringLiteral("actionMenuEsc"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, -1, -1);
        MainView = new QStackedWidget(centralWidget);
        MainView->setObjectName(QStringLiteral("MainView"));
        MainView->setEnabled(true);
        PageMainMenu = new QWidget();
        PageMainMenu->setObjectName(QStringLiteral("PageMainMenu"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(PageMainMenu->sizePolicy().hasHeightForWidth());
        PageMainMenu->setSizePolicy(sizePolicy);
        gridLayout_2 = new QGridLayout(PageMainMenu);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 1, 2, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_2->addItem(verticalSpacer_3, 2, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 1, 0, 1, 1);

        ButtonNewGame = new QPushButton(PageMainMenu);
        ButtonNewGame->setObjectName(QStringLiteral("ButtonNewGame"));
        ButtonNewGame->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(ButtonNewGame->sizePolicy().hasHeightForWidth());
        ButtonNewGame->setSizePolicy(sizePolicy1);
        QFont font;
        font.setPointSize(48);
        ButtonNewGame->setFont(font);
        ButtonNewGame->setFlat(false);

        gridLayout_2->addWidget(ButtonNewGame, 1, 1, 1, 1);

        ButtonLoadGame = new QPushButton(PageMainMenu);
        ButtonLoadGame->setObjectName(QStringLiteral("ButtonLoadGame"));
        sizePolicy1.setHeightForWidth(ButtonLoadGame->sizePolicy().hasHeightForWidth());
        ButtonLoadGame->setSizePolicy(sizePolicy1);
        ButtonLoadGame->setFont(font);
        ButtonLoadGame->setFlat(false);

        gridLayout_2->addWidget(ButtonLoadGame, 3, 1, 1, 1);

        ButtonConnect = new QPushButton(PageMainMenu);
        ButtonConnect->setObjectName(QStringLiteral("ButtonConnect"));
        sizePolicy1.setHeightForWidth(ButtonConnect->sizePolicy().hasHeightForWidth());
        ButtonConnect->setSizePolicy(sizePolicy1);
        ButtonConnect->setFont(font);
        ButtonConnect->setAutoDefault(false);
        ButtonConnect->setFlat(false);

        gridLayout_2->addWidget(ButtonConnect, 5, 1, 1, 1);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_5, 6, 1, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_2, 0, 1, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_2->addItem(verticalSpacer_4, 4, 1, 1, 1);

        MainView->addWidget(PageMainMenu);
        PageTableView = new QWidget();
        PageTableView->setObjectName(QStringLiteral("PageTableView"));
        sizePolicy.setHeightForWidth(PageTableView->sizePolicy().hasHeightForWidth());
        PageTableView->setSizePolicy(sizePolicy);
        verticalLayout_3 = new QVBoxLayout(PageTableView);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setSizeConstraint(QLayout::SetDefaultConstraint);
        verticalLayout_3->setContentsMargins(0, 0, -1, -1);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetFixedSize);
        horizontalLayout->setContentsMargins(-1, -1, 0, -1);
        TableViewTitle = new QLabel(PageTableView);
        TableViewTitle->setObjectName(QStringLiteral("TableViewTitle"));
        QFont font1;
        font1.setPointSize(36);
        TableViewTitle->setFont(font1);

        horizontalLayout->addWidget(TableViewTitle);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        ButtonTableViewBack = new QPushButton(PageTableView);
        ButtonTableViewBack->setObjectName(QStringLiteral("ButtonTableViewBack"));

        horizontalLayout->addWidget(ButtonTableViewBack);


        verticalLayout_3->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_3->addItem(verticalSpacer);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalSpacer_4 = new QSpacerItem(9, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);

        TableViewGeneral = new QTableView(PageTableView);
        TableViewGeneral->setObjectName(QStringLiteral("TableViewGeneral"));
        QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(TableViewGeneral->sizePolicy().hasHeightForWidth());
        TableViewGeneral->setSizePolicy(sizePolicy2);
        TableViewGeneral->setMinimumSize(QSize(600, 300));
        TableViewGeneral->setFrameShadow(QFrame::Raised);
        TableViewGeneral->setLineWidth(3);
        TableViewGeneral->setEditTriggers(QAbstractItemView::AnyKeyPressed|QAbstractItemView::DoubleClicked);
        TableViewGeneral->setAlternatingRowColors(true);
        TableViewGeneral->setSelectionMode(QAbstractItemView::SingleSelection);
        TableViewGeneral->setSelectionBehavior(QAbstractItemView::SelectRows);
        TableViewGeneral->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        TableViewGeneral->setShowGrid(false);
        TableViewGeneral->setWordWrap(false);
        TableViewGeneral->setCornerButtonEnabled(false);
        TableViewGeneral->verticalHeader()->setVisible(false);

        horizontalLayout_3->addWidget(TableViewGeneral);


        verticalLayout_3->addLayout(horizontalLayout_3);

        MainView->addWidget(PageTableView);
        PageLobby = new QWidget();
        PageLobby->setObjectName(QStringLiteral("PageLobby"));
        verticalLayout_5 = new QVBoxLayout(PageLobby);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, -1, -1);
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalLayout_7->setSizeConstraint(QLayout::SetFixedSize);
        horizontalLayout_7->setContentsMargins(-1, -1, 0, -1);
        label = new QLabel(PageLobby);
        label->setObjectName(QStringLiteral("label"));
        label->setFont(font1);

        horizontalLayout_7->addWidget(label);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_6);

        ButtonLobbyBack = new QPushButton(PageLobby);
        ButtonLobbyBack->setObjectName(QStringLiteral("ButtonLobbyBack"));

        horizontalLayout_7->addWidget(ButtonLobbyBack);


        verticalLayout_5->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        horizontalSpacer_9 = new QSpacerItem(9, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_9);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalSpacer_10 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_10);

        PlayerSlot_1 = new QPushButton(PageLobby);
        PlayerSlot_1->setObjectName(QStringLiteral("PlayerSlot_1"));
        QFont font2;
        font2.setPointSize(20);
        PlayerSlot_1->setFont(font2);

        verticalLayout_2->addWidget(PlayerSlot_1);

        verticalSpacer_9 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_9);

        PlayerSlot_2 = new QPushButton(PageLobby);
        PlayerSlot_2->setObjectName(QStringLiteral("PlayerSlot_2"));
        PlayerSlot_2->setFont(font2);

        verticalLayout_2->addWidget(PlayerSlot_2);

        verticalSpacer_8 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_8);

        PlayerSlot_3 = new QPushButton(PageLobby);
        PlayerSlot_3->setObjectName(QStringLiteral("PlayerSlot_3"));
        PlayerSlot_3->setFont(font2);

        verticalLayout_2->addWidget(PlayerSlot_3);

        verticalSpacer_7 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_7);

        PlayerSlot_4 = new QPushButton(PageLobby);
        PlayerSlot_4->setObjectName(QStringLiteral("PlayerSlot_4"));
        PlayerSlot_4->setFont(font2);

        verticalLayout_2->addWidget(PlayerSlot_4);


        verticalLayout_6->addLayout(verticalLayout_2);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_6);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        StepTimeValue = new QLineEdit(PageLobby);
        StepTimeValue->setObjectName(QStringLiteral("StepTimeValue"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(StepTimeValue->sizePolicy().hasHeightForWidth());
        StepTimeValue->setSizePolicy(sizePolicy3);
        StepTimeValue->setMaximumSize(QSize(80, 16777215));
        QFont font3;
        font3.setPointSize(18);
        StepTimeValue->setFont(font3);
        StepTimeValue->setAcceptDrops(false);
        StepTimeValue->setInputMethodHints(Qt::ImhNone);
        StepTimeValue->setMaxLength(4);

        horizontalLayout_5->addWidget(StepTimeValue);

        StepTimeDescriptor = new QLabel(PageLobby);
        StepTimeDescriptor->setObjectName(QStringLiteral("StepTimeDescriptor"));
        StepTimeDescriptor->setFont(font3);

        horizontalLayout_5->addWidget(StepTimeDescriptor);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_5);

        ButtonLobbyLevelSelect = new QPushButton(PageLobby);
        ButtonLobbyLevelSelect->setObjectName(QStringLiteral("ButtonLobbyLevelSelect"));
        ButtonLobbyLevelSelect->setFont(font3);

        horizontalLayout_5->addWidget(ButtonLobbyLevelSelect);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_7);

        CheckBoxLobbyClosed = new QCheckBox(PageLobby);
        CheckBoxLobbyClosed->setObjectName(QStringLiteral("CheckBoxLobbyClosed"));
        CheckBoxLobbyClosed->setFont(font3);

        horizontalLayout_5->addWidget(CheckBoxLobbyClosed);


        verticalLayout_6->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        pushButton_2 = new QPushButton(PageLobby);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setFont(font);

        horizontalLayout_6->addWidget(pushButton_2);


        verticalLayout_6->addLayout(horizontalLayout_6);


        horizontalLayout_8->addLayout(verticalLayout_6);


        verticalLayout_5->addLayout(horizontalLayout_8);

        MainView->addWidget(PageLobby);
        PageGame = new QWidget();
        PageGame->setObjectName(QStringLiteral("PageGame"));
        verticalLayout_7 = new QVBoxLayout(PageGame);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        verticalLayout_7->setContentsMargins(-1, -1, 0, 0);
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        playerLabel_1 = new QLabel(PageGame);
        playerLabel_1->setObjectName(QStringLiteral("playerLabel_1"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(playerLabel_1->sizePolicy().hasHeightForWidth());
        playerLabel_1->setSizePolicy(sizePolicy4);
        QFont font4;
        font4.setPointSize(14);
        playerLabel_1->setFont(font4);
        playerLabel_1->setMouseTracking(false);
        playerLabel_1->setFrameShape(QFrame::StyledPanel);
        playerLabel_1->setFrameShadow(QFrame::Sunken);
        playerLabel_1->setAlignment(Qt::AlignCenter);

        horizontalLayout_9->addWidget(playerLabel_1);

        playerLabel_2 = new QLabel(PageGame);
        playerLabel_2->setObjectName(QStringLiteral("playerLabel_2"));
        sizePolicy4.setHeightForWidth(playerLabel_2->sizePolicy().hasHeightForWidth());
        playerLabel_2->setSizePolicy(sizePolicy4);
        playerLabel_2->setFont(font4);
        playerLabel_2->setMouseTracking(false);
        playerLabel_2->setFrameShape(QFrame::StyledPanel);
        playerLabel_2->setFrameShadow(QFrame::Sunken);
        playerLabel_2->setAlignment(Qt::AlignCenter);

        horizontalLayout_9->addWidget(playerLabel_2);

        playerLabel_3 = new QLabel(PageGame);
        playerLabel_3->setObjectName(QStringLiteral("playerLabel_3"));
        sizePolicy4.setHeightForWidth(playerLabel_3->sizePolicy().hasHeightForWidth());
        playerLabel_3->setSizePolicy(sizePolicy4);
        playerLabel_3->setFont(font4);
        playerLabel_3->setMouseTracking(false);
        playerLabel_3->setFrameShape(QFrame::StyledPanel);
        playerLabel_3->setFrameShadow(QFrame::Sunken);
        playerLabel_3->setAlignment(Qt::AlignCenter);

        horizontalLayout_9->addWidget(playerLabel_3);

        playerLabel_4 = new QLabel(PageGame);
        playerLabel_4->setObjectName(QStringLiteral("playerLabel_4"));
        sizePolicy4.setHeightForWidth(playerLabel_4->sizePolicy().hasHeightForWidth());
        playerLabel_4->setSizePolicy(sizePolicy4);
        playerLabel_4->setFont(font4);
        playerLabel_4->setCursor(QCursor(Qt::ArrowCursor));
        playerLabel_4->setFrameShape(QFrame::StyledPanel);
        playerLabel_4->setFrameShadow(QFrame::Sunken);
        playerLabel_4->setAlignment(Qt::AlignCenter);

        horizontalLayout_9->addWidget(playerLabel_4);


        verticalLayout_7->addLayout(horizontalLayout_9);

        GameView = new ZoomGraphicsView(PageGame);
        GameView->setObjectName(QStringLiteral("GameView"));
        sizePolicy2.setHeightForWidth(GameView->sizePolicy().hasHeightForWidth());
        GameView->setSizePolicy(sizePolicy2);
        GameView->setMinimumSize(QSize(800, 600));
        GameView->setFocusPolicy(Qt::StrongFocus);
        GameView->setAcceptDrops(false);
        GameView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        GameView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        GameView->setInteractive(false);
        GameView->setRenderHints(QPainter::HighQualityAntialiasing|QPainter::SmoothPixmapTransform|QPainter::TextAntialiasing);
        GameView->setDragMode(QGraphicsView::ScrollHandDrag);

        verticalLayout_7->addWidget(GameView);

        verticalSpacer_11 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_7->addItem(verticalSpacer_11);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(-1, -1, -1, 0);
        CommandInput = new QLineEdit(PageGame);
        CommandInput->setObjectName(QStringLiteral("CommandInput"));
        CommandInput->setInputMethodHints(Qt::ImhNone);

        horizontalLayout_4->addWidget(CommandInput);

        LabelCommandStatus = new QLabel(PageGame);
        LabelCommandStatus->setObjectName(QStringLiteral("LabelCommandStatus"));

        horizontalLayout_4->addWidget(LabelCommandStatus);

        ButtonSendCommand = new QPushButton(PageGame);
        ButtonSendCommand->setObjectName(QStringLiteral("ButtonSendCommand"));

        horizontalLayout_4->addWidget(ButtonSendCommand);

        ButtonCommandHistory = new QPushButton(PageGame);
        ButtonCommandHistory->setObjectName(QStringLiteral("ButtonCommandHistory"));

        horizontalLayout_4->addWidget(ButtonCommandHistory);


        verticalLayout_7->addLayout(horizontalLayout_4);

        MainView->addWidget(PageGame);

        verticalLayout->addWidget(MainView);

        MainWindow->setCentralWidget(centralWidget);
#ifndef QT_NO_SHORTCUT
        StepTimeDescriptor->setBuddy(StepTimeValue);
#endif // QT_NO_SHORTCUT

        retranslateUi(MainWindow);

        MainView->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Bludiste 2014", 0));
        actionGameEsc->setText(QApplication::translate("MainWindow", "GameEsc", 0));
        actionGameEsc->setShortcut(QApplication::translate("MainWindow", "Esc", 0));
        actionMenuEsc->setText(QApplication::translate("MainWindow", "MenuEsc", 0));
        actionMenuEsc->setShortcut(QApplication::translate("MainWindow", "Esc", 0));
        ButtonNewGame->setText(QApplication::translate("MainWindow", "Start new game", 0));
        ButtonLoadGame->setText(QApplication::translate("MainWindow", "Load game", 0));
        ButtonConnect->setText(QApplication::translate("MainWindow", "Connect", 0));
        TableViewTitle->setText(QApplication::translate("MainWindow", "Selection", 0));
        ButtonTableViewBack->setText(QApplication::translate("MainWindow", "Back", 0));
        label->setText(QApplication::translate("MainWindow", "Lobby", 0));
        ButtonLobbyBack->setText(QApplication::translate("MainWindow", "Back", 0));
        PlayerSlot_1->setText(QApplication::translate("MainWindow", "Slot player 1", 0));
        PlayerSlot_2->setText(QApplication::translate("MainWindow", "Slot player 2", 0));
        PlayerSlot_3->setText(QApplication::translate("MainWindow", "Slot player 3", 0));
        PlayerSlot_4->setText(QApplication::translate("MainWindow", "Slot player 4", 0));
        StepTimeDescriptor->setText(QApplication::translate("MainWindow", "Step time", 0));
        ButtonLobbyLevelSelect->setText(QApplication::translate("MainWindow", "Select level", 0));
#ifndef QT_NO_TOOLTIP
        CheckBoxLobbyClosed->setToolTip(QApplication::translate("MainWindow", "If checked, players can't connect during game.", 0));
#endif // QT_NO_TOOLTIP
        CheckBoxLobbyClosed->setText(QApplication::translate("MainWindow", "Close game", 0));
        pushButton_2->setText(QApplication::translate("MainWindow", "Start game", 0));
        playerLabel_1->setText(QApplication::translate("MainWindow", "XXXXXXXXXXXXXXXX", 0));
        playerLabel_2->setText(QApplication::translate("MainWindow", "XXXXXXXXXXXXXXXX", 0));
        playerLabel_3->setText(QApplication::translate("MainWindow", "XXXXXXXXXXXXXXXX", 0));
        playerLabel_4->setText(QApplication::translate("MainWindow", "XXXXXXXXXXXXXXXX", 0));
        LabelCommandStatus->setText(QApplication::translate("MainWindow", "Status", 0));
        ButtonSendCommand->setText(QApplication::translate("MainWindow", "Send", 0));
        ButtonCommandHistory->setText(QApplication::translate("MainWindow", "History", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
