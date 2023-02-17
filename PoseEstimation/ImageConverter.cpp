#include "ImageConverter.h"

void ImageConverter::LoadCvMat(const cv::Mat& frame, const bool changeColorOrder)
{
	if (frame.empty() || Texture != 0)
		return;

	Mat = frame;

	if (changeColorOrder)
		cv::cvtColor(Mat, Mat, cv::COLOR_BGR2RGB);

	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Mat.cols, Mat.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, Mat.ptr());
}

void ImageConverter::UpdateMat(const cv::Mat& frame, const bool changeColorOrder)
{
	// if does not have the same size then i need to recreate the texture from scratch
	if (frame.size() != Mat.size())
	{
		Clear();
		Mat.release();
		LoadCvMat(frame);
	}

	if (changeColorOrder)
		cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

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

std::unique_ptr<ImVec2> ImageConverter::GetSize() const
{
	return std::make_unique<ImVec2>(static_cast<float>(Mat.cols), static_cast<float>(Mat.rows));
}

