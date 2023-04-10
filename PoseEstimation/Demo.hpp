#pragma once
#include "PoseEstimation.hpp"
#include "Runnable.hpp"
#include "Camera.hpp"
#include "World.hpp"

class Demo final : public Runnable
{
	// Variables
	World DemoWorld{};

	std::shared_ptr<cv::dnn::Net> Net = PoseEstimation::CreateDnnNet(Config::ProtoTextPath, Config::CaffeModel, Config::DnnMode);
	Camera FrontCamera;
	std::unique_ptr<PoseEstimation> FrontCameraEstimator;
	std::atomic<bool> FrontCameraCancellationToken = false;
	inline static std::unique_ptr<std::jthread> FrontCameraUpdateThread = std::make_unique<std::jthread>();

	inline static bool Show3dModel = true;
	inline static bool Show3dModelControls = true;

	// Methods
public:
	explicit Demo();
	~Demo() override;

private:
	void Loop() override;
	void HandleInput() override;
};
