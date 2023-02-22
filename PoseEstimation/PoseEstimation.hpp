#pragma once

#include "ImageConverter.hpp"
#include <opencv2/core.hpp>
#include <opencv2/dnn.hpp>
#include "KeyPoint.hpp"
#include <filesystem>
#include "Config.hpp"

class PoseEstimation
{	
	// Variables
	std::unique_ptr<cv::dnn::Net> Net;
	std::unique_ptr<ImageConverter> Image;

	// Methods
public:
	PoseEstimation(const std::string& protoTextPath, const std::string& caffeModel, const DnnTargetMode& dnnMode);
	void Create(const std::unique_ptr<cv::Mat>& mat) const;
	void Update(const std::unique_ptr<cv::Mat>& mat) const;
	void* GetTexture() const;
private:
	std::unique_ptr<cv::Mat> FindPose(const std::unique_ptr<cv::Mat>& mat) const;
	void ConvertPoseToImage(const std::unique_ptr<cv::Mat>& outputBlob, const std::unique_ptr<cv::Mat>& mat) const;
	//void SplitNetOutputBlobToParts(cv::Mat& netOutputBlob, const cv::Size& targetSize, std::vector<cv::Mat>& netOutputParts) const;
};