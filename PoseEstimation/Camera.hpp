#pragma once

#include <opencv2/core/cuda.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>
#include "ImageConverter.hpp"

class Camera
{
public:
//private:
    // Variables
    std::unique_ptr<cv::VideoCapture> VideoCapture;
    std::unique_ptr<ImageConverter> Image;
    std::unique_ptr<cv::Mat> Frame;

public:
    // Methods
    explicit Camera(const int cameraIndex, const ImVec2& size, const cv::VideoCaptureAPIs& cameraApi);
    explicit Camera(const char* cameraIp, const ImVec2& size, const cv::VideoCaptureAPIs& cameraApi);
    void SetUpdateCameraThread(std::unique_ptr<std::jthread>& thread) const;
    void UpdateImage() const;
    bool OpenCamera() const;
    void* GetTexture() const;
    std::unique_ptr<cv::Mat> GetMat() const;
};
