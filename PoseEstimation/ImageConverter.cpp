#include "ImageConverter.hpp"

ImageConverter::ImageConverter() : Texture(0), Width(0), Height(0) { }

void ImageConverter::LoadMat(const void* frameId, const int& width, const int& height)
{
	if (Texture != 0)
		return;

	Width = width;
	Height = height;

	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, frameId);
}

void ImageConverter::UpdateMat(const void* frameId, const int& width, const int& height)
{
	// if does not have the same size then i need to recreate the texture from scratch
	if (Width != width || Height != height)
	{
		Clear();
		LoadMat(frameId, width, height);
		return;
	}

	// image must have same size
	glBindTexture(GL_TEXTURE_2D, Texture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, frameId);
}

void ImageConverter::LoadMat(const cv::Mat& frame)
{
	LoadMat(frame.ptr(), frame.cols, frame.rows);
}


void ImageConverter::UpdateMat(const cv::Mat& frame)
{
	UpdateMat(frame.ptr(), frame.cols, frame.rows);
}

// clear texture and release all memory associated with it
void ImageConverter::Clear()
{	
	glDeleteTextures(1, &Texture);
	Texture = 0;
}

void* ImageConverter::GetTexture() const
{
	return reinterpret_cast<void*>(static_cast<intptr_t>(Texture));
}