#pragma once
#include "Libs.hpp"

class ImageConverter
{
	// Variables
	GLuint Texture;
	int Width;
	int Height;

public:
	// Methods
	ImageConverter();
	void LoadMat(const void* frameId, const int& width, const int& height);
	void LoadMat(const cv::Mat& frame);
	void UpdateMat(const void* frameId, const int& width, const int& height);
	void UpdateMat(const cv::Mat& frame);
	void* GetTexture() const;
	void Clear();
};
