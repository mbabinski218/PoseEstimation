#pragma once
#include "Libs.hpp"

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
    ~FrameBuffer();
    void Bind() const;
	void Unbind() const;
    void Clear();
    void* GetTexture() const;
};
