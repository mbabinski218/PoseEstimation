#include "ImageConverter.hpp"

ImageConverter::ImageConverter() : Texture(0), Size(cv::Size(0, 0)) { }

void ImageConverter::LoadMat(const void* frameId, const int& width, const int& height)
{
	if (Texture != 0)
		return;

	Size = cv::Size(width, height);

	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, frameId);
}

void ImageConverter::LoadMat(const cv::Mat& frame)
{
	LoadMat(frame.ptr(), frame.cols, frame.rows);
}

void ImageConverter::UpdateMat(const cv::Mat& frame)
{
	// if does not have the same size then i need to recreate the texture from scratch
	if (frame.size() != Size)
	{
		Clear();
		LoadMat(frame);
		return;
	}
	
	// image must have same size
	glBindTexture(GL_TEXTURE_2D, Texture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frame.cols, frame.rows, GL_RGB, GL_UNSIGNED_BYTE, frame.ptr());
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