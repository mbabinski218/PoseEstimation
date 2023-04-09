#pragma once

class Runnable
{
public:
	void Render();
	virtual ~Runnable() = default;

protected:
	Runnable();

	virtual void Loop() = 0;
	virtual void HandleInput() = 0;
	void InitGlfw() const;
	void InitWindow();
	void InitGlew() const;
	void InitOpenGL() const;
	void Shutdown() const;

	static void GlfwErrorCallback(int error, const char* description);
	static void FrameBufferResizeCallback(GLFWwindow* window, int fbW, int fbH);
	static void GLAPIENTRY GlMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam);

	GLFWwindow* Window{};
	ImVec2 Padding{};
	float DeltaTime{0.0f};
	float LastFrame{0.0f};
};
