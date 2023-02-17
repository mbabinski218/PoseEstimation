#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include "ImageConverter.h"

class PoseEstimation
{
public:
//private:
    // Variables
    std::unique_ptr<cv::VideoCapture> Camera;
    std::unique_ptr<cv::Mat> Frame;
    std::mutex UpdateMutex;

//public:
	std::unique_ptr<ImageConverter> Image;

    // Methods
    PoseEstimation(const int cameraIndex, const int x, const int y);
    bool OpenCamera() const;
    void UpdateImage();
};
