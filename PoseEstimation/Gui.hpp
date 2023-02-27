#pragma once

#include "Mesh.hpp"
#include "imgui_impl_opengl3.h"
#include "PoseEstimation.hpp"
#include "imgui_impl_glfw.h"
#include "Camera.hpp"
#include "Config.hpp"

class Gui
{
public:
	// Variables
	GLFWwindow* Window;
	std::shared_ptr<Config> GuiConfig;
	std::shared_ptr<cv::dnn::Net> Net;
	std::unique_ptr<Camera> FrontCamera;
	std::unique_ptr<Camera> BackCamera;
	std::unique_ptr<PoseEstimation> FrontCameraEstimator;
	std::unique_ptr<PoseEstimation> BackCameraEstimator;
	std::unique_ptr<Mesh> Model;

	int FrameBufferWidth;
	int FrameBufferHeight;

	inline static ImVec4 BgColor = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);
	inline static std::unique_ptr<std::jthread> FrontCameraUpdateThread = std::make_unique<std::jthread>();
	inline static std::unique_ptr<std::jthread> BackCameraUpdateThread = std::make_unique<std::jthread>();
	inline static bool ShowPoseEstimation = false;
	inline static bool Show3dModel = true;

	// Methods
	explicit Gui(const std::shared_ptr<Config>& config);
	~Gui();
	void Render() const;

private:
	void Loop() const;
	void InitGlfw() const;
	void InitWindow();
	void InitGlew() const;
	void InitOpenGL() const;
	void InitCamera() const;

	static void GlfwErrorCallback(int error, const char* description);
	static void FrameBufferResizeCallback(GLFWwindow* window, int fbW, int fbH);
};
