#pragma once
#include "Utility.hpp"
#include "Bone.hpp"

struct Config
{
	inline static const char* const						   Title			 = "Pose estimation";
	inline static const char* const						   GlslVersion		 = "#version 430 core";
	inline static ImVec2 constexpr						   WindowSize		 = {1920, 1080};
	inline static ImVec2 constexpr						   FrameSize		 = {1280, 720};
	inline static ImVec2 constexpr						   FrontCameraSize   = {640, 480};
	inline static ImVec2 constexpr						   BackCameraSize    = {640, 480};
	inline static cv::VideoCaptureAPIs constexpr		   CameraApi		 = cv::CAP_ANY;
	inline static CameraLinker<int> constexpr			   FrontCameraLinker = {0};
	inline static CameraLinker<const char*> constexpr	   BackCameraLinker  = {"http://192.168.0.158:4747/video"};
	inline static const char* const						   ProtoTextPath	 = R"(\models\pose\coco\pose_deploy_linevec.prototxt)";
	inline static const char* const						   CaffeModel	     = R"(\models\pose\coco\pose_iter_440000.caffemodel)";
	inline static DnnTargetMode constexpr				   DnnMode			 = DnnTargetMode::CPU;		
	inline static double constexpr						   ThreshHold		 = 0.1;
	inline static const char* const						   FontPath		     = R"(\fonts\CascadiaMono.ttf)";
	inline static const char* const						   VertexCorePath	 = R"(\shaders\shader.vert)";		
	inline static const char* const						   FragmentCorePath  = R"(\shaders\shader.frag)";			
	inline static const char* const						   ModelPath		 = R"(\3dModels\Man.glb)";
	inline static int constexpr							   MaxBoneInfluence  = 3;
	inline static int constexpr							   MaxBones			 = 100;
	inline static int constexpr							   PoseParts	     = 14;		
	inline static std::vector<std::vector<BoneType>> const PosePairs      = {{NECK, RIGHT_ARM}, {NECK, LEFT_ARM},
																				{RIGHT_ARM, RIGHT_FOREARM}, {RIGHT_FOREARM, RIGHT_HAND},
																				{LEFT_ARM, LEFT_FOREARM}, {LEFT_FOREARM, LEFT_HAND},
																				{NECK, RIGHT_UP_LEG}, {NECK, LEFT_UP_LEG},
																				{RIGHT_UP_LEG, RIGHT_LEG}, {RIGHT_LEG, RIGHT_FOOT},
																				{LEFT_UP_LEG, LEFT_LEG}, {LEFT_LEG, LEFT_FOOT},
																				{HEAD, NECK}};
	inline static std::map<BoneType, std::string> const    BoneTypeNames  = {{HEAD,		    "Head"},
																				{NECK,		    "Neck"},
																				{LEFT_ARM,	    "LeftArm"},
																				{LEFT_FOREARM,  "LeftForeArm"},
																				{LEFT_HAND,     "LeftHand"},
																				{RIGHT_ARM,     "RightArm"},
																				{RIGHT_FOREARM, "RightForeArm"},
																				{RIGHT_HAND,    "RightHand"},
																				{LEFT_UP_LEG,   "LeftUpLeg"},
																				{LEFT_LEG,      "LeftLeg"},
																				{LEFT_FOOT,     "LeftFoot"},
																				{RIGHT_UP_LEG,  "RightUpLeg"},
																				{RIGHT_LEG,     "RightLeg"},
																				{RIGHT_FOOT,    "RightFoot"}};
};