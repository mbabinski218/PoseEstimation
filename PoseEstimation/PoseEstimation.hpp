#pragma once

#include <opencv2/dnn.hpp>
#include <opencv2/core.hpp>
#include "ImageConverter.hpp"
#include <filesystem>

class PoseEstimation
{	
	// Variables
	std::unique_ptr<cv::dnn::Net> Net;
	std::unique_ptr<ImageConverter> Image;

	// Methods
public:
	PoseEstimation(const std::string& protoTextPath, const std::string& caffeModel);
	void Create(const std::unique_ptr<cv::Mat>& mat) const;
	void Update(const std::unique_ptr<cv::Mat>& mat) const;
	void* GetTexture() const;
	static void ToPose(std::unique_ptr<cv::Mat>& mat);
private:
	std::unique_ptr<cv::Mat> FindPose(const std::unique_ptr<cv::Mat>& mat) const;
};