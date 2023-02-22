#include <boost/exception/diagnostic_information.hpp> 
#include "Config.hpp"
#include <iostream>
#include "Gui.hpp"

#ifdef _WIN32
#include "Windows.h"
#endif

std::shared_ptr<Config> Setup()
{
	auto glslVersion       = "#version 430 core";
    auto title             = "Pose estimation";
    auto windowSize        = ImVec2(1920, 1080);
    auto frontCameraSize   = ImVec2(640, 480);
    auto backCameraSize    = ImVec2(640, 480);
    auto videoCaptureApi   = cv::CAP_ANY;
    auto protoTextPath     = R"(\models\pose\coco\pose_deploy_linevec.prototxt)";
    auto caffeModel        = R"(\models\pose\coco\pose_iter_440000.caffemodel)";
    auto dnnMode           = DnnTargetMode::Cpu;
    auto frontCameraLinker = CameraLinker(0);
    auto backCameraLinker  = CameraLinker("http://192.168.1.108:4747/video");

    return std::make_shared<Config>(glslVersion, title, windowSize, frontCameraSize, backCameraSize, 
        videoCaptureApi, protoTextPath, caffeModel, dnnMode, frontCameraLinker, backCameraLinker);
}

int main()
{
    try
    {
        const auto config = Setup();
        const auto gui = std::make_unique<Gui>(config);
    	gui->Render();
    }
    catch(...)
    {
#ifdef _WIN32
        const auto h = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(h, 4);
        std::cout << "[EXCEPTION] " << boost::current_exception_diagnostic_information();
        SetConsoleTextAttribute(h, 7);
#else 
        std::cout << "[EXCEPTION] " << boost::current_exception_diagnostic_information();
#endif

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}