#pragma once

// Copyright (C) Force67 2019

#include <kern/proc.h>
#include <memory>
#include <string>

#include <QApplication>
#include <QFontDatabase>
#include <QIcon>

#include "ui/mainwindow.h"

class deltaCore : public QApplication {
  Q_OBJECT
public:
  using argvList = std::vector<std::string>;

  deltaCore(int &, char **);

  bool init();
  void boot(std::string &fromdir);

  argvList argv;
private:
  bool headless{false};
  std::unique_ptr<mainWindow> window;
  std::unique_ptr<krnl::proc> proc;
};