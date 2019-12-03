#pragma once

// Copyright (C) Force67 2019

#include <string>
#include <memory>
#include <kernel/Process.h>

#include <QApplication>
#include <QFontDatabase>
#include <QIcon>

class deltaCore : public QApplication
{
	Q_OBJECT
public:
	deltaCore(int&, char**);

	void boot(const std::string& fromdir);
private:
	std::unique_ptr<krnl::Process> proc;
};