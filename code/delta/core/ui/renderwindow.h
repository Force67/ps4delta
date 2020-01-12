#pragma once

// Copyright (C) Force67 2019

#include <QWidget>
#include <QWindow>

#include "gfx/gfx_base.h"

class renderWindow : public QWindow, public gfx::frameBase {
  Q_OBJECT

public:
  renderWindow();
  ~renderWindow();

  void toggleFullscreen() override;
  void takeScreenshot(std::vector<uint8_t> &data, uint32_t sizeX,
                      uint32_t sizeY) override;

private:
  bool inFullscreen{false};
  bool showFps{false};
};