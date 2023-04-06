#pragma once
#include "Libs.hpp"
#include "ImageConverter.hpp"
#include "Config.hpp"

class PoseEstimation
{	
	// Variables
	std::shared_ptr<cv::dnn::Net> Net;
	std::unique_ptr<ImageConverter> Image;
	std::unique_ptr<cv::Mat> OutputBlob;

	std::vector<std::vector<int>> PosePairs;
	double ThreshHold;
	cv::Size2f CameraSize;
	int PoseParts;

	// Methods
public:
	explicit PoseEstimation(std::shared_ptr<cv::dnn::Net> net, const ImVec2& size, const int& poseParts, std::vector<std::vector<int>> posePairs, const double& threshHold);
	void Update(const std::unique_ptr<cv::Mat>& mat) const;
	void* GetTexture() const;

	static std::shared_ptr<cv::dnn::Net> CreateDnnNet(const std::string& protoTextPath, const std::string& caffeModel, const DnnTargetMode& dnnMode);

private:
	void FindPose(const std::unique_ptr<cv::Mat>& mat) const;
	void AddPoseToImage(const std::unique_ptr<cv::Mat>& mat) const;
};