#pragma once
#include "ImageConverter.hpp"
#include "Skeleton.hpp"
#include "Config.hpp"
#include "World.hpp"

class PoseEstimation
{	
public:
	static std::shared_ptr<cv::dnn::Net> CreateDnnNet(const std::string& protoTextPath, const std::string& caffeModel, const DnnTargetMode& dnnMode);

	explicit PoseEstimation(std::shared_ptr<cv::dnn::Net> net, const ImVec2& size) : Net(std::move(net)), CameraSize(Converter::ToSize2f(size)) {}
	void Update(cv::Mat mat, World& world);
	[[nodiscard]] void* GetTexture() const;

private:
	void FindPose(const cv::Mat& mat);
	void AddPoseToImage(const cv::Mat& mat);
	void UpdateWorldModels(World& world) const;

	std::unique_ptr<cv::Mat> OutputBlob{};
	std::shared_ptr<cv::dnn::Net> Net{};
	ImageConverter Image{};
	cv::Size2f CameraSize{};
	Skeleton PoseSkeleton{};
};