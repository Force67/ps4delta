#pragma once

// Copyright (C) Force67 2019

#include <QDockWidget>
#include <QDesktopWidget>
#include <QMainWindow>

namespace Ui {
	class main_window;
}

class mainWindow : public QMainWindow
{
	Q_OBJECT

public:
	mainWindow(QWidget*);
	~mainWindow();

	void init();
private:
	void createConnects();
	void bootELF();

	Ui::main_window* ui;
};