#pragma once

#include "glew/glew.h"
#include <GLFW/glfw3.h>

class FrameBuffer
{
	// Variables
    GLuint FBO;
    GLuint TextureId;
    GLuint DepthId;
    int Width;
    int Height;

	// Methods
public:
    FrameBuffer(const int& width, const int& height);
    void Bind() const;
	void Unbind() const;
    void Clear();
    void* GetTexture() const;
};
