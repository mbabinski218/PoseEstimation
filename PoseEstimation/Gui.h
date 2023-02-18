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
	const static ImVec2 WindowSize;
	const static ImVec2 FrontCameraSize;
	const static ImVec2 BackCameraSize;
	const static cv::VideoCaptureAPIs CameraApi;
	static std::unique_ptr<PoseEstimation> FrontCamera;
	static std::unique_ptr<PoseEstimation> BackCamera;
	static std::unique_ptr<std::jthread> FrontCameraUpdateThread;
	static std::unique_ptr<std::jthread> BackCameraUpdateThread;

	// Methods
	static void Render();
private:
	static void Init();
	static void Loop();
};
