#pragma once

#include "imgui_impl_opengl3.h"
#include "PoseEstimation.hpp"
#include "imgui_impl_glfw.h"
#include <corecrt_math.h>
#include <GLFW/glfw3.h>
#include <exception>
#include <imgui.h>
#include <cstdio>
#include "Camera.hpp"
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
	const static std::string ProtoTextPath;
	const static std::string CaffeModel;
	static bool ShowPoseEstimation;
	static std::unique_ptr<Camera> FrontCamera;
	static std::unique_ptr<Camera> BackCamera;
	static std::unique_ptr<std::jthread> FrontCameraUpdateThread;
	static std::unique_ptr<std::jthread> BackCameraUpdateThread;
	static std::unique_ptr<PoseEstimation> FrontPoseEstimation;
	//static std::unique_ptr<PoseEstimation> BackPoseEstimation;

	// Methods
	static void Render();
private:
	static void Init();
	static void Loop();
};
