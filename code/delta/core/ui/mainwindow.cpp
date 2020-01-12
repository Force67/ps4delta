
// Copyright (C) Force67 2019

#include <QApplication>
#include <QDesktopWidget>
#include <QDockWidget>
#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QMimeData>
#include <QScreen>
#include <QVBoxLayout>

#include "logframe.h"
#include "mainwindow.h"
#include "renderwindow.h"

#include "qtgen/ui_mainwindow.h"

#include "logger/logger.h"

mainWindow::mainWindow(deltaCore &core)
    : QMainWindow(nullptr), core(core), ui(new Ui::main_window()) {
  setWindowTitle(rsc_productname);
}

mainWindow::~mainWindow() { delete ui; }

void mainWindow::init() {
  ui->setupUi(this);

  setAcceptDrops(true);
  // setWindowIcon()

  createConnects();

  rendView = new renderWindow;
  rendView->hide();

  auto *log = new logFrame;
  auto *dock = new QDockWidget(tr("Log"), this);
  dock->setWidget(log);

  addDockWidget(Qt::RightDockWidgetArea, dock);

  show();
}

void mainWindow::boot(const std::string &what) {
  // show the render window
  rendView->show();
}

void mainWindow::onBootSelection() {
  // TODO: pause system
  QString filePath = QFileDialog::getOpenFileName(
      this, tr("Select (S)ELF to boot"), "",
      tr("(S)ELF files (*BOOT.BIN *.elf *.self);;"
         "ELF files (BOOT.BIN *.elf);;"
         "SELF files (EBOOT.BIN *.self);;"
         "BOOT files (*BOOT.BIN);"
         "BIN files (*.bin);;",
         Q_NULLPTR, QFileDialog::DontResolveSymlinks));

  if (filePath == nullptr) {
    // resume system
  }

  auto qs = QFileInfo(filePath).canonicalFilePath();
  auto *strData = qs.toUtf8().constData();
  LOG_INFO("fileDialog: Booting {}", strData);

  // todo: fix str conversion
  boot(strData);
}

void mainWindow::onEnginePause() { LOG_INFO("Pause Requested"); }

void mainWindow::onEngineStop() { LOG_INFO("Stop Requested"); }

void mainWindow::createConnects() {
  connect(ui->bootAct, &QAction::triggered, this, &mainWindow::onBootSelection);
  connect(ui->enginePauseAct, &QAction::triggered, this,
          &mainWindow::onEnginePause);
  connect(ui->engineStopAct, &QAction::triggered, this,
          &mainWindow::onEnginePause);
}

void mainWindow::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_F6) {
    if (!isPaused)
      onEnginePause();
  }
}

void mainWindow::keyReleaseEvent(QKeyEvent *event) {}