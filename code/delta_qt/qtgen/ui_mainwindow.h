/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_main_window
{
public:
    QAction *bootAct;
    QAction *enginePauseAct;
    QAction *engineStopAct;
    QAction *exitAct;
    QWidget *centralWidget;
    QWidget *sizeSliderContainer;
    QHBoxLayout *horizontalLayout;
    QToolBar *toolBar;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *bootRecentMenu;
    QMenu *menuEngine;
    QMenu *menuConfig;

    void setupUi(QMainWindow *main_window)
    {
        if (main_window->objectName().isEmpty())
            main_window->setObjectName(QString::fromUtf8("main_window"));
        main_window->resize(1058, 580);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(main_window->sizePolicy().hasHeightForWidth());
        main_window->setSizePolicy(sizePolicy);
        main_window->setMinimumSize(QSize(4, 0));
        main_window->setAutoFillBackground(false);
        main_window->setAnimated(true);
        main_window->setDockNestingEnabled(true);
        main_window->setDockOptions(QMainWindow::AllowNestedDocks|QMainWindow::AllowTabbedDocks|QMainWindow::AnimatedDocks|QMainWindow::GroupedDragging);
        bootAct = new QAction(main_window);
        bootAct->setObjectName(QString::fromUtf8("bootAct"));
        enginePauseAct = new QAction(main_window);
        enginePauseAct->setObjectName(QString::fromUtf8("enginePauseAct"));
        enginePauseAct->setEnabled(false);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Icons/pause.png"), QSize(), QIcon::Normal, QIcon::Off);
        enginePauseAct->setIcon(icon);
        engineStopAct = new QAction(main_window);
        engineStopAct->setObjectName(QString::fromUtf8("engineStopAct"));
        engineStopAct->setEnabled(false);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/Icons/stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        engineStopAct->setIcon(icon1);
        exitAct = new QAction(main_window);
        exitAct->setObjectName(QString::fromUtf8("exitAct"));
        centralWidget = new QWidget(main_window);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        sizeSliderContainer = new QWidget(centralWidget);
        sizeSliderContainer->setObjectName(QString::fromUtf8("sizeSliderContainer"));
        sizeSliderContainer->setGeometry(QRect(280, 10, 181, 31));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(sizeSliderContainer->sizePolicy().hasHeightForWidth());
        sizeSliderContainer->setSizePolicy(sizePolicy1);
        horizontalLayout = new QHBoxLayout(sizeSliderContainer);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(14, 0, 14, 0);
        main_window->setCentralWidget(centralWidget);
        toolBar = new QToolBar(main_window);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(toolBar->sizePolicy().hasHeightForWidth());
        toolBar->setSizePolicy(sizePolicy2);
        toolBar->setContextMenuPolicy(Qt::PreventContextMenu);
        toolBar->setMovable(false);
        toolBar->setAllowedAreas(Qt::TopToolBarArea);
        toolBar->setIconSize(QSize(30, 30));
        toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        toolBar->setFloatable(false);
        main_window->addToolBar(Qt::TopToolBarArea, toolBar);
        menuBar = new QMenuBar(main_window);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1058, 17));
        menuBar->setContextMenuPolicy(Qt::PreventContextMenu);
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        bootRecentMenu = new QMenu(menuFile);
        bootRecentMenu->setObjectName(QString::fromUtf8("bootRecentMenu"));
        bootRecentMenu->setEnabled(true);
        bootRecentMenu->setToolTipsVisible(true);
        menuEngine = new QMenu(menuBar);
        menuEngine->setObjectName(QString::fromUtf8("menuEngine"));
        menuConfig = new QMenu(menuBar);
        menuConfig->setObjectName(QString::fromUtf8("menuConfig"));
        main_window->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEngine->menuAction());
        menuBar->addAction(menuConfig->menuAction());
        menuFile->addAction(bootAct);
        menuFile->addAction(bootRecentMenu->menuAction());
        menuFile->addSeparator();
        menuFile->addSeparator();
        menuFile->addSeparator();
        menuFile->addSeparator();
        menuFile->addAction(exitAct);
        bootRecentMenu->addSeparator();
        menuEngine->addAction(enginePauseAct);
        menuEngine->addAction(engineStopAct);
        menuEngine->addSeparator();
        menuConfig->addSeparator();
        menuConfig->addSeparator();

        retranslateUi(main_window);

        QMetaObject::connectSlotsByName(main_window);
    } // setupUi

    void retranslateUi(QMainWindow *main_window)
    {
        main_window->setWindowTitle(QCoreApplication::translate("main_window", "PS4Delta", nullptr));
        bootAct->setText(QCoreApplication::translate("main_window", "Boot ELF", nullptr));
#if QT_CONFIG(tooltip)
        bootAct->setToolTip(QCoreApplication::translate("main_window", "Boot ELF", nullptr));
#endif // QT_CONFIG(tooltip)
        enginePauseAct->setText(QCoreApplication::translate("main_window", "Pause", nullptr));
#if QT_CONFIG(tooltip)
        enginePauseAct->setToolTip(QCoreApplication::translate("main_window", "Start Engine", nullptr));
#endif // QT_CONFIG(tooltip)
        engineStopAct->setText(QCoreApplication::translate("main_window", "Stop", nullptr));
#if QT_CONFIG(tooltip)
        engineStopAct->setToolTip(QCoreApplication::translate("main_window", "Stop Engine", nullptr));
#endif // QT_CONFIG(tooltip)
        exitAct->setText(QCoreApplication::translate("main_window", "Exit", nullptr));
#if QT_CONFIG(tooltip)
        exitAct->setToolTip(QCoreApplication::translate("main_window", "Exit PS4Delta", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        exitAct->setStatusTip(QCoreApplication::translate("main_window", "Exit the application", nullptr));
#endif // QT_CONFIG(statustip)
        toolBar->setWindowTitle(QCoreApplication::translate("main_window", "Show tool bar", nullptr));
        menuFile->setTitle(QCoreApplication::translate("main_window", "File", nullptr));
        bootRecentMenu->setTitle(QCoreApplication::translate("main_window", "Boot Recent", nullptr));
        menuEngine->setTitle(QCoreApplication::translate("main_window", "Engine", nullptr));
        menuConfig->setTitle(QCoreApplication::translate("main_window", "Config", nullptr));
    } // retranslateUi

};

namespace Ui {
    class main_window: public Ui_main_window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
