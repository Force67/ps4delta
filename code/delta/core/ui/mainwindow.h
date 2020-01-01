#pragma once

/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

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