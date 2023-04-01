#pragma once
#include "Libs.hpp"

class Runnable
{
protected:
	GLFWwindow* Window;
	ImVec2 Padding;

public:
	void Render();
	virtual ~Runnable() = default;

protected:
	Runnable();

	virtual void Loop() = 0;
	virtual void HandleInput() const = 0;
	void InitGlfw() const;
	void InitWindow();
	void InitGlew() const;
	void InitOpenGL() const;
	void Shutdown() const;

	static void GlfwErrorCallback(int error, const char* description);
	static void FrameBufferResizeCallback(GLFWwindow* window, int fbW, int fbH);
	static void GLAPIENTRY GlMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam);
};

void Run(const Runnable& runnable);