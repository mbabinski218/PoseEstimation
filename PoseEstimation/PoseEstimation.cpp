#include "PoseEstimation.h"

// Variables
const cv::VideoCaptureAPIs PoseEstimation::CameraAPI = cv::CAP_ANY;

// Methods
void Init(std::unique_ptr<cv::VideoCapture>& camera, std::unique_ptr<cv::Mat>& frame, std::unique_ptr<ImageConverter>& image)
{
	camera = std::make_unique<cv::VideoCapture>();
	frame = std::make_unique<cv::Mat>();
	image = std::make_unique<ImageConverter>();
}

PoseEstimation::PoseEstimation(const int cameraIndex) 
{
	Init(Camera, Frame, Image);
	Camera->open(cameraIndex, CameraAPI);
}

PoseEstimation::PoseEstimation(const char* cameraIp)
{
	Init(Camera, Frame, Image);
	Camera->open(cameraIp, CameraAPI);	
}

void PoseEstimation::UpdateImage() const
{
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