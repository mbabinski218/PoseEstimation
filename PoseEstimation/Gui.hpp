#pragma once
#include "Libs.hpp"
#include "Config.hpp"
#include "Mesh.hpp"
#include "PoseEstimation.hpp"
#include "Camera.hpp"
#include "ImageConverter.hpp"
#include "Input.hpp"

class Gui
{
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
	ImVec2 Padding;

	inline static ImVec4 BgColor = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);
	inline static std::unique_ptr<std::jthread> FrontCameraUpdateThread = std::make_unique<std::jthread>();
	inline static std::unique_ptr<std::jthread> BackCameraUpdateThread = std::make_unique<std::jthread>();
	inline static bool ShowPoseEstimation = false;
	inline static bool Show3dModel = true;
	inline static bool Show3dModelControls = true;

	// Methods
public:
	explicit Gui(const std::shared_ptr<Config>& config);
	~Gui();
	void Render() const;

private:
	void Loop() const;
	void HandleInput() const;
	void InitGlfw() const;
	void InitWindow();
	void InitGlew() const;
	void InitOpenGL() const;
	void InitCamera() const;

	static void GlfwErrorCallback(int error, const char* description);
	static void FrameBufferResizeCallback(GLFWwindow* window, int fbW, int fbH);
	static void GLAPIENTRY GlMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam);
};