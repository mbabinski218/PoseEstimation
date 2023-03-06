#pragma once
#include "Libs.hpp"
#include "Utility.hpp"

struct Config
{
	static std::shared_ptr<Config> Get()
	{
		auto glslVersion	   = "#version 430 core";
		auto title			   = "Pose estimation";
		auto dnnMode		   = DnnTargetMode::CPU;
		auto protoTextPath     = R"(\models\pose\coco\pose_deploy_linevec.prototxt)";
		auto caffeModelPath    = R"(\models\pose\coco\pose_iter_440000.caffemodel)";
		auto modelObjPath	   = R"(\3dModels\Man.obj)";
		auto vertexCorePath    = R"(\shaders\vertex.glsl)";
		auto fragmentCorePath  = R"(\shaders\fragment.glsl)";
		auto fontPath		   = R"(\fonts\CascadiaMono.ttf)";
		auto windowSize		   = ImVec2(1920, 1080);
		auto frontCameraSize   = ImVec2(640, 480);
		auto backCameraSize    = ImVec2(640, 480);
		auto videoCaptureApi   = cv::CAP_ANY;
		auto frontCameraLinker = CameraLinker(0);
		auto backCameraLinker  = CameraLinker("http://192.168.0.158:4747/video");
		auto threshHold		   = 0.1;
		auto poseParts		   = 18;
		auto posePairs		   = std::vector<std::vector<int>>({ {1,2}, {1,5}, {2,3},
																	  {3,4}, {5,6}, {6,7},
																	  {1,8}, {8,9}, {9,10},
																	  {1,11}, {11,12}, {12,13},
																	  {1,0}, {0,14},
																	  {14,16}, {0,15}, {15,17} });

		return std::make_shared<Config>(glslVersion, title, windowSize, frontCameraSize, backCameraSize, videoCaptureApi,
			protoTextPath, caffeModelPath, modelObjPath, dnnMode, frontCameraLinker, backCameraLinker, poseParts, posePairs, 
			threshHold, vertexCorePath, fragmentCorePath, fontPath);
	}

	const char* GlslVersion;
	const char* Title;
	ImVec2 WindowSize;
	ImVec2 FrontCameraSize;
	ImVec2 BackCameraSize;
	cv::VideoCaptureAPIs CameraApi;
	std::string ProtoTextPath;
	std::string CaffeModel;
	std::string ModelObjPath;
	DnnTargetMode DnnMode;
	CameraLinker<int> FrontCameraLinker;
	CameraLinker<const char*> BackCameraLinker;
	int PoseParts;
	std::vector<std::vector<int>> PosePairs;
	double ThreshHold;
	const char* VertexCorePath;
	const char* FragmentCorePath;
	std::string FontPath;
};