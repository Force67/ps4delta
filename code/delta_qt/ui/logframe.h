#pragma once

/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <QPlainTextEdit>

class qtlogSink;

class logFrame : public QPlainTextEdit {
    Q_OBJECT

public:
    logFrame();
    void addLine(const QString&);

private:
    void update();

    qtlogSink* sink;
};