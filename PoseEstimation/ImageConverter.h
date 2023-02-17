#pragma once

#include "opencv2/imgproc.hpp"
#include <GLFW/glfw3.h>
#include "imgui.h"

class ImageConverter
{
	// Variables
	GLuint Texture;
	cv::Mat Mat;

public:
	// Methods
	ImageConverter() : Texture(0) { }
	void LoadCvMat(const cv::Mat& frame, const bool changeColorOrder = true);
	void UpdateMat(const cv::Mat& frame, const bool changeColorOrder = true);	
	std::unique_ptr<ImVec2> GetSize() const;
	void* GetTexture() const;
	void Clear();
};
