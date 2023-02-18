#include "PoseEstimation.h"

// Methods
void Init(std::unique_ptr<cv::VideoCapture>& camera, std::unique_ptr<cv::Mat>& frame, std::unique_ptr<ImageConverter>& image)
{
	camera = std::make_unique<cv::VideoCapture>();
	frame = std::make_unique<cv::Mat>();
	image = std::make_unique<ImageConverter>();
}

PoseEstimation::PoseEstimation(const int cameraIndex, const ImVec2& size, const cv::VideoCaptureAPIs& cameraApi)
{
	Init(Camera, Frame, Image);
	Camera->open(cameraIndex, cameraApi);
}

PoseEstimation::PoseEstimation(const char* cameraIp, const ImVec2& size, const cv::VideoCaptureAPIs& cameraApi)
{
	Init(Camera, Frame, Image);
	Camera->open(cameraIp, cameraApi);	
}

bool PoseEstimation::OpenCamera() const
{
	const bool isOpened = Camera->isOpened();

	if (isOpened)
	{
		Camera->read(*Frame);
		Image->LoadCvMat(*Frame);
	}

	return isOpened;
}

void PoseEstimation::SetUpdateCameraThread(std::unique_ptr<std::jthread>& thread) const
{
	thread = std::make_unique<std::jthread>([=]() {
		while (true)
		{
			Camera->grab();
		}
	});
}

void PoseEstimation::UpdateImage() const
{
	Camera->retrieve(*Frame);
	Image->UpdateMat(*Frame);
}

void* PoseEstimation::GetTexture() const
{
	return Image->GetTexture();
}