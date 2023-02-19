#pragma once

#include <opencv2/core/cuda.hpp>
#include "opencv2/imgproc.hpp"
#include <GLFW/glfw3.h>
#include "imgui.h"

class ImageConverter
{
public:
//private:
	// Variables
	GLuint Texture;
	cv::Size Size;

public:
	// Methods
	ImageConverter() : Texture(0) { }
	//void LoadCvMat(const cv::cuda::GpuMat& frame, const bool changeColorOrder = true);
	void LoadCvMat(const cv::Mat& frame);
	//void UpdateMat(const cv::cuda::GpuMat& frame, const bool changeColorOrder = true);
	void UpdateMat(const cv::Mat& frame);
	void* GetTexture() const;
	void Clear();
};
