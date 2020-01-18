#pragma once

// Copyright (C) Force67 2019

#include <QDesktopWidget>
#include <QDockWidget>
#include <QMainWindow>

namespace Ui {
class main_window;
}

class renderWindow;
class deltaCore;

class mainWindow : public QMainWindow {
  Q_OBJECT

public:
  mainWindow(deltaCore &);
  ~mainWindow();

  void init();

private:
  bool isPaused{false};

  void boot(const std::string &what);
  void createConnects();

  /*callbacks*/
  void onBootSelection();
  void onEnginePause();
  void onEngineStop();

  // drag'n drop
  void dragEnterEvent(QDragEnterEvent *event) override;
  void dragMoveEvent(QDragMoveEvent *event) override;
  void dragLeaveEvent(QDragLeaveEvent *event) override;
  void dropEvent(QDropEvent *event) override;

  void keyPressEvent(QKeyEvent *) override;
  void keyReleaseEvent(QKeyEvent *) override;

  deltaCore &core;
  Ui::main_window *ui;
  renderWindow *rendView;
};