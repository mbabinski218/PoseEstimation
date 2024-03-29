#pragma once

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
	explicit FrameBuffer(const ImVec2& size);
    ~FrameBuffer();
    void Bind() const;
	void Unbind() const;
    void Clear();
    void* GetTexture() const;
};
