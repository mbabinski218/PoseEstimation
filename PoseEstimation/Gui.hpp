#pragma once
#include "Runnable.hpp"
#include "Mesh.hpp"
#include "PoseEstimation.hpp"
#include "Camera.hpp"
#include "Model.hpp"

class Gui : public Runnable
{
	// Variables
	std::shared_ptr<cv::dnn::Net> Net;
	std::unique_ptr<Camera> FrontCamera;
	std::unique_ptr<Camera> BackCamera;
	std::unique_ptr<PoseEstimation> FrontCameraEstimator;
	std::unique_ptr<PoseEstimation> BackCameraEstimator;

	std::atomic<bool> FrontCameraCancellationToken;
	std::atomic<bool> BackCameraCancellationToken;

	inline static std::unique_ptr<std::jthread> FrontCameraUpdateThread = std::make_unique<std::jthread>();
	inline static std::unique_ptr<std::jthread> BackCameraUpdateThread = std::make_unique<std::jthread>();
	inline static bool ShowPoseEstimation = false;
	inline static bool Show3dModel = true;
	inline static bool Show3dModelControls = true;

	// Methods
public:
	explicit Gui();
	~Gui() override;

private:
	void Loop() override;
	void HandleInput() const override;

	void InitCamera() const;
	void ShutdownCamera();
};
