#include "Camera.hpp"

// Methods
void Init(std::unique_ptr<cv::VideoCapture>& videoCapture, std::unique_ptr<cv::Mat>& frame, std::unique_ptr<ImageConverter>& image)
{
	videoCapture = std::make_unique<cv::VideoCapture>();
	frame = std::make_unique<cv::Mat>();
	image = std::make_unique<ImageConverter>();
}

Camera::Camera(const int cameraIndex, const ImVec2& size, const cv::VideoCaptureAPIs& cameraApi)
{
	Init(VideoCapture, Frame, Image);
	VideoCapture->open(cameraIndex, cameraApi);
}

Camera::Camera(const char* cameraIp, const ImVec2& size, const cv::VideoCaptureAPIs& cameraApi)
{
	Init(VideoCapture, Frame, Image);
	VideoCapture->open(cameraIp, cameraApi);	
}

bool Camera::OpenCamera() const
{
	const bool isOpened = VideoCapture->isOpened();

	if (isOpened)
	{
		VideoCapture->read(*Frame);
		Image->LoadCvMat(*Frame);
	}

	return isOpened;
}

void Camera::SetUpdateCameraThread(std::unique_ptr<std::jthread>& thread) const
{
	thread = std::make_unique<std::jthread>([=]() {
		while (true)
		{
			VideoCapture->read(*Frame);
			//cvtColor(*Frame, *Frame, cv::COLOR_BGR2RGB);
		}
	});
}

void Camera::UpdateImage() const
{	
	Image->UpdateMat(*Frame);
}

void* Camera::GetTexture() const
{
	return Image->GetTexture();
}

std::unique_ptr<cv::Mat> Camera::GetMat() const
{
	return std::make_unique<cv::Mat>(*Frame);
}
