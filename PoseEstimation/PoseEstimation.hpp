#pragma once

#include <opencv2/core/cuda.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>
#include "ImageConverter.hpp"

class PoseEstimation
{
public:
//private:
    // Variables
    std::unique_ptr<cv::VideoCapture> Camera;
    std::unique_ptr<ImageConverter> Image;
    //std::unique_ptr<cv::cuda::GpuMat> Frame;
    std::unique_ptr<cv::Mat> Frame;

public:
    // Methods
    explicit PoseEstimation(const int cameraIndex, const ImVec2& size, const cv::VideoCaptureAPIs& cameraApi);
    explicit PoseEstimation(const char* cameraIp, const ImVec2& size, const cv::VideoCaptureAPIs& cameraApi);
    void SetUpdateCameraThread(std::unique_ptr<std::jthread>& thread) const;
    void UpdateImage() const;
    bool OpenCamera() const;
    void* GetTexture() const;
};
