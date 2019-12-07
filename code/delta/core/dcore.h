#pragma once

// Copyright (C) Force67 2019

#include <string>
#include <memory>
#include <kernel/Process.h>

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
	void boot(const std::string& fromdir);
private:
	bool headless{ false };
	std::unique_ptr<mainWindow> window;
	std::unique_ptr<krnl::Process> proc;
};