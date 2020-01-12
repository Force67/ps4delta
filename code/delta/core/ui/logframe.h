#pragma once

// Copyright (C) Force67 2019

#include <QPlainTextEdit>

class qtlogSink;

class logFrame : public QPlainTextEdit {
  Q_OBJECT

public:
  logFrame();
  void addLine(const QString &);

private:
  void update();

  qtlogSink *sink;
};