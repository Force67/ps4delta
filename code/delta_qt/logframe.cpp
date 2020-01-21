
/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <QScrollbar>
#include <QTimer>

#include "logframe.h"

#include <logger/logger.h>

class qtlogSink final : public utl::logBase {
public:
    qtlogSink(logFrame* frame) : frame(frame) {}

    const char* getName() override {
        return "qtsink";
    }

    void write(const utl::logEntry& entry) override {
        // auto msg = formatLogEntry(entry).append(1, '\n');

        // put it in a queue to avoid thread context issues
        // queue.emplace_back(msg);
    }

    std::string* getNextPacket() {
        if (queue.empty())
            return nullptr;

        auto& last = queue.back();
        queue.pop_back();
        return &last;
    }

private:
    logFrame* frame;
    std::vector<std::string> queue;
};

logFrame::logFrame() {
    setReadOnly(true);

    // todo: fix this mess
    sink = static_cast<qtlogSink*>(utl::addLogSink(std::make_unique<qtlogSink>(this)));

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &logFrame::update);
    timer->start(10);
}

void logFrame::addLine(const QString& line) {
    appendPlainText(line);

    // auto* bar = verticalScrollBar();
    // bar->setValue(bar->maximum());
}

void logFrame::update() {
    /*auto* msg = sink->getNextPacket();
    if (msg) {
            addLine(QString::fromUtf8(msg->data(), msg->size()));
    }*/
}