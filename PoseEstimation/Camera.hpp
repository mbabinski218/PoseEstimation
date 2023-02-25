#pragma once

#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>
#include "ImageConverter.hpp"
#include "Config.hpp"

class Camera
{
    // Variables
    std::unique_ptr<cv::VideoCapture> VideoCapture;
    std::unique_ptr<ImageConverter> Image;
    std::unique_ptr<cv::Mat> Frame;

public:
    // Methods
    Camera(const CameraLinker<int>& linker, const ImVec2& size, const cv::VideoCaptureAPIs& cameraApi);
    Camera(const CameraLinker<const char*>& linker, const ImVec2& size, const cv::VideoCaptureAPIs& cameraApi);
    void SetUpdateCameraThread(std::unique_ptr<std::jthread>& thread) const;
    void Read() const;
    void UpdateImage() const;
    bool OpenCamera() const;
    void* GetTexture() const;
    std::unique_ptr<cv::Mat> GetMat() const;
};
