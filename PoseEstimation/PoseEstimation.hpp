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
	std::shared_ptr<cv::dnn::Net> Net;
	std::unique_ptr<ImageConverter> Image;

	std::vector<std::vector<int>> PosePairs;
	double ThreshHold = 0.01f;
	cv::Size2f Size;
	int PoseParts;

	// Methods
public:
	explicit PoseEstimation(std::shared_ptr<cv::dnn::Net> net, const ImVec2& size, const int& poseParts, 
		const std::vector<std::vector<int>>& posePairs, const double& threshHold);
	void Update(const std::unique_ptr<cv::Mat>& mat) const;
	void* GetTexture() const;

	static std::shared_ptr<cv::dnn::Net> CreateDnnNet(const std::string& protoTextPath, const std::string& caffeModel, const DnnTargetMode& dnnMode);

private:
	std::unique_ptr<cv::Mat> FindPose(const std::unique_ptr<cv::Mat>& mat) const;
	void AddPoseToImage(const std::unique_ptr<cv::Mat>& outputBlob, const std::unique_ptr<cv::Mat>& mat) const;
};