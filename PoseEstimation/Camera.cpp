#include "stdafx.h"
#include "Camera.hpp"

// Methods
void Init(std::unique_ptr<cv::VideoCapture>& videoCapture, std::unique_ptr<cv::Mat>& frame, std::unique_ptr<ImageConverter>& image)
{
	videoCapture = std::make_unique<cv::VideoCapture>();
	frame = std::make_unique<cv::Mat>();
	image = std::make_unique<ImageConverter>();
}

Camera::Camera(const CameraLinker<int>& linker, const ImVec2& size)
{
	Init(VideoCapture, Frame, Image);
	VideoCapture->open(linker.Link, Config::CameraApi);
}

Camera::Camera(const CameraLinker<const char*>& linker, const ImVec2& size)
{
	Init(VideoCapture, Frame, Image);
	VideoCapture->open(linker.Link, Config::CameraApi);
}

bool Camera::OpenCamera() const
{
	const bool isOpened = VideoCapture->isOpened();

	if (isOpened)
	{
		Read();
		Image->LoadMat(*Frame);
	}

	return isOpened;
}

void Camera::SetUpdateCameraThread(std::unique_ptr<std::jthread>& thread, const std::atomic<bool>& cancellationToken) const
{
	thread = std::make_unique<std::jthread>([&]()
	{
		while(!cancellationToken)
		{
			Read();			
		}
	});
}

void Camera::Read() const
{
	VideoCapture->read(*Frame);
	cvtColor(*Frame, *Frame, cv::COLOR_BGR2RGB);
}

void Camera::UpdateImage() const
{
	Image->UpdateMat(*Frame);
}

void* Camera::GetTexture() const
{
	return Image->GetTexture();
}

cv::Mat Camera::GetMat() const
{
	return cv::Mat(*Frame);
}
