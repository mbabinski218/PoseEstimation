#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include "ImageConverter.h"

class PoseEstimation
{
private:
    // Variables
    const static cv::VideoCaptureAPIs CameraAPI;
    std::unique_ptr<cv::VideoCapture> Camera;
    std::unique_ptr<cv::Mat> Frame;

public:
    std::unique_ptr<ImageConverter> Image;

    // Methods
    explicit PoseEstimation(const int cameraIndex);
    explicit PoseEstimation(const char* cameraIp);
    bool OpenCamera() const;
    void UpdateImage() const;
};
