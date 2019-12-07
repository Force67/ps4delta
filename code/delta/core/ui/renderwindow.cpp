
// Copyright (C) Force67 2019

#include "renderwindow.h"
#include "logger/logger.h"

renderWindow::renderWindow() :
	QWindow()
{
	// require 720p
	setMinimumWidth(1280);
	setMinimumHeight(720);
	setTitle("PS4Delta - GameWindow - [Vulkan]");
}

renderWindow::~renderWindow()
{}

void renderWindow::toggleFullscreen()
{
	/*if (inFullscreen)
		//setVisibility(Windowed);
	else
		//setVisibility(FullScreen);*/

	inFullscreen = !inFullscreen;
}

void renderWindow::takeScreenshot(std::vector<uint8_t>& data, uint32_t sizeX, uint32_t sizeY)
{
	LOG_UNIMPLEMENTED;
}
