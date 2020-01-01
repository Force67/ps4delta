#pragma once

/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <string>
#include <memory>
#include <kern/proc.h>

#include <QApplication>
#include <QFontDatabase>
#include <QIcon>

#include "ui/mainwindow.h"

class deltaCore : public QApplication
{
	Q_OBJECT
public:
	deltaCore(int&, char**);

	bool init();
	void boot(std::string& fromdir);
private:
	bool headless{ false };
	std::unique_ptr<mainWindow> window;
	std::unique_ptr<krnl::proc> proc;
};