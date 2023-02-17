#pragma once

#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include <corecrt_math.h>
#include <GLFW/glfw3.h>
#include <exception>
#include <imgui.h>
#include <cstdio>
#include "PoseEstimation.h"
#include <thread>

class Gui
{
public:
	// Variables
	const static char* GlslVersion;
	const static char* Title;
	const static int Width;
	const static int Height;
	static PoseEstimation* FrontCamera;
	static std::unique_ptr<PoseEstimation> BackCamera;

	// Methods
	static void Render();

private:
	static void Init();
	static void Loop();
};
