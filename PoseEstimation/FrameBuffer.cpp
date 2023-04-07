#include "FrameBuffer.hpp"

FrameBuffer::FrameBuffer(const ImVec2& size) :
	FBO(0),
	TextureId(0),
	DepthId(0),
	Width(static_cast<int>(size.x)),
	Height(static_cast<int>(size.y))
{
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glCreateTextures(GL_TEXTURE_2D, 1, &TextureId);
    glBindTexture(GL_TEXTURE_2D, TextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureId, 0);

    glCreateTextures(GL_TEXTURE_2D, 1, &DepthId);
    glBindTexture(GL_TEXTURE_2D, DepthId);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, Width, Height);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, DepthId, 0);

    constexpr GLenum buffers[4] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(TextureId, buffers);

    Unbind();
}

FrameBuffer::~FrameBuffer()
{
    //Clear();
}

void FrameBuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glViewport(0, 0, Width, Height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FrameBuffer::Unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void* FrameBuffer::GetTexture() const
{
    return reinterpret_cast<void*>(static_cast<intptr_t>(TextureId));
}

void FrameBuffer::Clear()
{
    glDeleteFramebuffers(GL_FRAMEBUFFER, &FBO);
    glDeleteTextures(1, &TextureId);
    glDeleteTextures(1, &DepthId);
    TextureId = 0;
    DepthId = 0;
}