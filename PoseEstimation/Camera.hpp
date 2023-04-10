#pragma once
#include "Config.hpp"
#include "ImageConverter.hpp"

class Camera
{
    // Variables
    std::unique_ptr<cv::VideoCapture> VideoCapture;
    std::unique_ptr<ImageConverter> Image;
    std::unique_ptr<cv::Mat> Frame;

public:
    // Methods
    Camera(const CameraLinker<int>& linker, const ImVec2& size);
    Camera(const CameraLinker<const char*>& linker, const ImVec2& size);
    void SetUpdateCameraThread(std::unique_ptr<std::jthread>& thread, const std::atomic<bool>& cancellationToken) const;
    void Read() const;
    void UpdateImage() const;
    bool OpenCamera() const;
    void* GetTexture() const;
    cv::Mat GetMat() const;
};
