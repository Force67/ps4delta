#pragma once

// Copyright (C) Force67 2019

#include <QWidget>
#include <QWindow>

#include "gfx/gfxBase.h"

// for now we implement it as subview of the main window
class renderWindow : public QWindow, public gfx::frameBase
{
	Q_OBJECT

public:
	renderWindow();
	~renderWindow();

	void toggleFullscreen() override;
	void takeScreenshot(std::vector<uint8_t>& data, uint32_t sizeX, uint32_t sizeY);

private:
	bool inFullscreen{ false };
	bool showFps{ false };
};