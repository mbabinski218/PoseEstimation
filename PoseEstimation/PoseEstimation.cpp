#include "PoseEstimation.h"

PoseEstimation::PoseEstimation(const int cameraIndex, const int x, const int y) 
{
	Camera = std::make_unique<cv::VideoCapture>();
	Camera->open(cameraIndex, cv::CAP_ANY);
	//Camera->set(cv::CAP_PROP_FRAME_WIDTH, x);
	//Camera->set(cv::CAP_PROP_FRAME_HEIGHT, y);

	Frame = std::make_unique<cv::Mat>();
	Image = std::make_unique<ImageConverter>();
}

void PoseEstimation::UpdateImage()
{
	const std::lock_guard lock(UpdateMutex);
	Camera->read(*Frame);
	Image->UpdateMat(*Frame);
}

bool PoseEstimation::OpenCamera() const
{
	const bool isOpened = Camera->isOpened();

	if(isOpened)
	{
		Camera->read(*Frame);
		Image->LoadCvMat(*Frame);
	}

	return isOpened;
}


