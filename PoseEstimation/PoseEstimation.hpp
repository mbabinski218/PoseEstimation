#pragma once
#include "Config.hpp"
#include "ImageConverter.hpp"
#include "Skeleton.hpp"

class PoseEstimation
{	
public:
	static std::shared_ptr<cv::dnn::Net> CreateDnnNet(const std::string& protoTextPath, const std::string& caffeModel, const DnnTargetMode& dnnMode);

	explicit PoseEstimation(std::shared_ptr<cv::dnn::Net> net, const ImVec2& size, int poseParts, std::vector<std::vector<BoneType>> posePairs, double threshHold);
	void Update(const cv::Mat& mat);
	[[nodiscard]] void* GetTexture() const;

private:
	void FindPose(const cv::Mat& mat);
	void AddPoseToImage(const cv::Mat& mat);

	std::shared_ptr<cv::dnn::Net> Net{};
	ImageConverter Image{};
	cv::Mat OutputBlob{};

	cv::Size2f CameraSize{};
	double ThreshHold{};
	std::vector<std::vector<BoneType>> PosePairs{};
	int PoseParts{};

	Skeleton PoseSkeleton{};
};
