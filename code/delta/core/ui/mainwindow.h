#pragma once

// Copyright (C) Force67 2019

#include <QDockWidget>
#include <QDesktopWidget>
#include <QMainWindow>

namespace Ui {
	class main_window;
}

class renderWindow;
class deltaCore;

class mainWindow : public QMainWindow
{
	Q_OBJECT

public:
	mainWindow(deltaCore&);
	~mainWindow();

	void init();
private:
	bool isPaused{ false };

	void boot(const std::string& what);
	void createConnects();

	/*callbacks*/
	void onBootSelection();
	void onEnginePause();
	void onEngineStop();

	void keyPressEvent(QKeyEvent*) override;
	void keyReleaseEvent(QKeyEvent*) override;

	deltaCore& core;
	Ui::main_window* ui;
	renderWindow* rendView;
};