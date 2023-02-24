#pragma once

#include "opencv2/imgproc.hpp"
#include <GLFW/glfw3.h>

class ImageConverter
{
	// Variables
	GLuint Texture;
	cv::Size Size;

public:
	// Methods
	ImageConverter();
	void LoadMat(const void* frameId, const int& width, const int& height);
	void LoadMat(const cv::Mat& frame);
	void UpdateMat(const cv::Mat& frame);
	void* GetTexture() const;
	void Clear();
};
