/********************************************************************************
** Form generated from reading UI file 'dialoggamemenu.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGGAMEMENU_H
#define UI_DIALOGGAMEMENU_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DialogGameMenu
{
public:
    QVBoxLayout *verticalLayout;
    QPushButton *ButtonSaveGame;
    QPushButton *ButtonLeave;

    void setupUi(QDialog *DialogGameMenu)
    {
        if (DialogGameMenu->objectName().isEmpty())
            DialogGameMenu->setObjectName(QStringLiteral("DialogGameMenu"));
        DialogGameMenu->resize(320, 170);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DialogGameMenu->sizePolicy().hasHeightForWidth());
        DialogGameMenu->setSizePolicy(sizePolicy);
        DialogGameMenu->setModal(true);
        verticalLayout = new QVBoxLayout(DialogGameMenu);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        ButtonSaveGame = new QPushButton(DialogGameMenu);
        ButtonSaveGame->setObjectName(QStringLiteral("ButtonSaveGame"));
        QFont font;
        font.setPointSize(20);
        ButtonSaveGame->setFont(font);

        verticalLayout->addWidget(ButtonSaveGame);

        ButtonLeave = new QPushButton(DialogGameMenu);
        ButtonLeave->setObjectName(QStringLiteral("ButtonLeave"));
        ButtonLeave->setFont(font);

        verticalLayout->addWidget(ButtonLeave);


        retranslateUi(DialogGameMenu);

        QMetaObject::connectSlotsByName(DialogGameMenu);
    } // setupUi

    void retranslateUi(QDialog *DialogGameMenu)
    {
        DialogGameMenu->setWindowTitle(QApplication::translate("DialogGameMenu", "Game Menu", 0));
        ButtonSaveGame->setText(QApplication::translate("DialogGameMenu", "Save game", 0));
        ButtonLeave->setText(QApplication::translate("DialogGameMenu", "Leave", 0));
    } // retranslateUi

};

namespace Ui {
    class DialogGameMenu: public Ui_DialogGameMenu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGGAMEMENU_H
