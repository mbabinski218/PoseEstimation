#pragma once

#include <opencv2/videoio.hpp>
#include "imgui.h"

enum DnnTargetMode
{
	Cpu,
	Cuda
};

template<typename T>
struct CameraLinker
{
	T Link;
	explicit CameraLinker(T link) : Link(link) { }
};

struct Config
{
	const char* GlslVersion;
	const char* Title;
	ImVec2 WindowSize;
	ImVec2 FrontCameraSize;
	ImVec2 BackCameraSize;
	cv::VideoCaptureAPIs CameraApi;
	std::string ProtoTextPath;
	std::string CaffeModel;
	DnnTargetMode DnnMode;
	CameraLinker<int> FrontCameraLinker;
	CameraLinker<const char*> BackCameraLinker;
	int PoseParts;
	std::vector<std::vector<int>> PosePairs;
	double ThreshHold;

	Config(const char* glslVersion, const char* title, const ImVec2& windowSize, const ImVec2& frontCameraSize,
		const ImVec2& backCameraSize, const cv::VideoCaptureAPIs& cameraApi, const char* protoTextPath,
		const char* caffeModel, const DnnTargetMode& dnnMode, const CameraLinker<int>& frontCameraLinker,
		const CameraLinker<const char*>& backCameraLinker, const int& poseParts,
		std::vector<std::vector<int>> poseParis, const double& threshHold) : GlslVersion(glslVersion), Title(title),
		WindowSize(windowSize), FrontCameraSize(frontCameraSize), BackCameraSize(backCameraSize), CameraApi(cameraApi),
		ProtoTextPath(protoTextPath), CaffeModel(caffeModel), DnnMode(dnnMode), FrontCameraLinker(frontCameraLinker),
		BackCameraLinker(backCameraLinker), PoseParts(poseParts), PosePairs(std::move(poseParis)),
		ThreshHold(threshHold) { }
};