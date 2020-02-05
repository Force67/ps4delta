#pragma once

/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <QDesktopWidget>
#include <QDockWidget>
#include <QMainWindow>

#include <core.h>

namespace Ui {
class main_window;
}

class renderWindow;
class deltaCore;

class mainWindow final : public QMainWindow {
    Q_OBJECT

public:
    mainWindow(core::System&);
    ~mainWindow();

    void init();

private:
    bool isPaused{false};

    void boot(const std::string& what);
    void createConnects();

    /*callbacks*/
    void onBootSelection();
    void onEnginePause();
    void onEngineStop();

    // drag'n drop
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dragLeaveEvent(QDragLeaveEvent* event) override;
    void dropEvent(QDropEvent* event) override;

    void keyPressEvent(QKeyEvent*) override;
    void keyReleaseEvent(QKeyEvent*) override;

    Ui::main_window* ui;
    core::System& sys;
    //renderWindow* rendView;
};