#include "PoseEstimation.hpp"

#include <opencv2/highgui.hpp>

// Methods
PoseEstimation::PoseEstimation(const std::string& protoTextPath, const std::string& caffeModel)
	: Image(std::make_unique<ImageConverter>())
{
	const auto currentPath = std::filesystem::current_path().string();
	auto net = cv::dnn::readNetFromCaffe(currentPath + protoTextPath, currentPath + caffeModel);
	Net = std::make_unique<cv::dnn::Net>(net);

	if (net.empty())
		throw std::exception("Error when reading model");
}

std::unique_ptr<cv::Mat> PoseEstimation::FindPose(const std::unique_ptr<cv::Mat>& mat) const
{
	std::unique_ptr<cv::Mat> pose;
	const auto thread = std::make_unique<std::jthread>([&]() {
		Net->setInput(cv::dnn::blobFromImage(*mat));
		pose = std::make_unique<cv::Mat>(Net->forward());
	});
	thread->join();

	return pose;
}

void PoseEstimation::Create(const std::unique_ptr<cv::Mat>& mat) const
{
	const auto pose = FindPose(mat);
	
	Image->LoadCvMat(*pose);
}

void PoseEstimation::Update(const std::unique_ptr<cv::Mat>& mat) const 
{
	const auto pose = FindPose(mat);
	
	Image->UpdateMat(*pose);
	cv::imshow("pose", *pose);
}

void* PoseEstimation::GetTexture() const
{
	return Image->GetTexture();
}

void PoseEstimation::ToPose(std::unique_ptr<cv::Mat>& mat)
{
	throw std::logic_error("Not implemented");
}
