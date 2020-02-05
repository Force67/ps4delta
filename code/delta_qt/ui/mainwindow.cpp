
/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <QApplication>
#include <QDesktopWidget>
#include <QDockWidget>
#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QMimeData>
#include <QScreen>
#include <QVBoxLayout>

#include <utl/path.h>

#include "logframe.h"
#include "mainwindow.h"
#include "renderwindow.h"

#include "qtgen/ui_mainwindow.h"

#include "logger/logger.h"

#include "core.h"

mainWindow::mainWindow(core::System& sys)
    : QMainWindow(nullptr), ui(new Ui::main_window()), sys(sys) {
    setWindowTitle(rsc_productname);
    setAcceptDrops(true);
    ui->setupUi(this);
}

mainWindow::~mainWindow() {
    delete ui;
}

void mainWindow::init() {
    createConnects();

    // rendView = new renderWindow;
    // rendView->hide();

    auto* log = new logFrame;
    auto* dock = new QDockWidget(tr("Log"), this);
    dock->setWidget(log);

    addDockWidget(Qt::RightDockWidgetArea, dock);

    show();

    // this is here *temorarily* while we don't have a fw installer
    if (!utl::exists(utl::make_app_path(utl::data_dir, "lib/libkernel.sprx"))) {
        auto path = utl::make_app_path(utl::data_dir, "lib");

        QMessageBox msg(QMessageBox::Critical, tr("Firmware error"),
            tr("Failed to find the firmware\nPlease put it at %1").arg(QString::fromUtf8(path.c_str())));
        msg.setWindowFlags(msg.windowFlags() & (~Qt::WindowContextHelpButtonHint));
        msg.exec();
    }
}

void mainWindow::boot(const std::string& what) {
    // show the render window
    // rendView->show();
}

void mainWindow::onBootSelection() {
    // TODO: pause system
    QString filePath =
        QFileDialog::getOpenFileName(this, tr("Select (S)ELF to boot"), "",
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
    auto* strData = qs.toUtf8().constData();
    LOG_INFO("fileDialog: Booting {}", strData);

    // todo: fix str conversion
    boot(strData);
}

void mainWindow::onEnginePause() {
    LOG_INFO("Pause Requested");
}

void mainWindow::onEngineStop() {
    LOG_INFO("Stop Requested");
}

void mainWindow::createConnects() {
    connect(ui->bootAct, &QAction::triggered, this, &mainWindow::onBootSelection);
    connect(ui->enginePauseAct, &QAction::triggered, this, &mainWindow::onEnginePause);
    connect(ui->engineStopAct, &QAction::triggered, this, &mainWindow::onEnginePause);
}

void mainWindow::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_F6) {
        if (!isPaused)
            onEnginePause();
    }
}

void mainWindow::keyReleaseEvent(QKeyEvent* event) {}

static bool isValidFile(const QMimeData& md) {
    auto list = md.urls();

    // TODO: verify file type here

    // only allow one file
    if (list.size() > 1 || !list.size())
        return false;

    auto& url = list[0];
    const auto path = url.toLocalFile();
    const QFileInfo info = path;

    if (info.isDir())
        return false;

    return true;
}

void mainWindow::dragEnterEvent(QDragEnterEvent* event) {
    if (isValidFile(*event->mimeData()))
        event->accept();
}

void mainWindow::dragMoveEvent(QDragMoveEvent* event) {
    if (isValidFile(*event->mimeData()))
        event->accept();
}

void mainWindow::dragLeaveEvent(QDragLeaveEvent* event) {
    event->accept();
}

void mainWindow::dropEvent(QDropEvent* event) {
    auto& md = *event->mimeData();

    if (isValidFile(md)) {
        const auto path = md.urls()[0].toLocalFile();
        auto bytes = path.toUtf8();

        // this is kinda bad
        std::string name(bytes.constData());
        sys.load(name);
    }
}