
// Copyright (C) Force67 2019

#include <QApplication>
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QDockWidget>
#include <QDesktopWidget>
#include <QMimeData>
#include <QScreen>

#include "mainwindow.h"
#include "qtgen/ui_mainwindow.h"

#include "logger/logger.h"

mainWindow::mainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::main_window())
{}

mainWindow::~mainWindow()
{
	delete ui;
}

void mainWindow::init()
{
	ui->setupUi(this);

	setAcceptDrops(true);
	createConnects();
	setWindowTitle(rsc_productname);
	show();
}

void mainWindow::bootELF()
{
	LOG_INFO("BOOTELF Requested");
}

void mainWindow::createConnects()
{
	connect(ui->bootAct, &QAction::triggered, this, &mainWindow::bootELF);
}