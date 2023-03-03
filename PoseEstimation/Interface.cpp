#include "Libs.hpp"
#include "Config.hpp"
#include "Gui.hpp"

std::shared_ptr<Config> Setup()
{
	auto glslVersion       = "#version 430 core";
    auto title             = "Pose estimation";
    auto dnnMode           = DnnTargetMode::Cpu;
    auto protoTextPath     = R"(\models\pose\coco\pose_deploy_linevec.prototxt)";
    auto caffeModelPath    = R"(\models\pose\coco\pose_iter_440000.caffemodel)";
    auto modelObjPath      = R"(\3dModels\Man.obj)";
    auto vertexCorePath    = R"(\shaders\vertex_core.glsl)";
    auto fragmentCorePath  = R"(\shaders\fragment_core.glsl)";
    auto windowSize        = ImVec2(1920, 1080);
    auto modelSize         = ImVec2(800, 800);
    auto frontCameraSize   = ImVec2(640, 480);
    auto backCameraSize    = ImVec2(640, 480);
    auto videoCaptureApi   = cv::CAP_ANY;
    auto frontCameraLinker = CameraLinker(0);
    auto backCameraLinker  = CameraLinker("http://192.168.0.158:4747/video");
    auto threshHold        = 0.1;
    auto poseParts         = 18;
    auto posePairs         = std::vector<std::vector<int>>({{1,2}, {1,5}, {2,3},
															      {3,4}, {5,6}, {6,7},
															      {1,8}, {8,9}, {9,10},
																  {1,11}, {11,12}, {12,13},
                                                                  {1,0}, {0,14},
													              {14,16}, {0,15}, {15,17}});

    return std::make_shared<Config>(glslVersion, title, windowSize, frontCameraSize, backCameraSize, videoCaptureApi, 
        protoTextPath, caffeModelPath, dnnMode, frontCameraLinker, backCameraLinker, poseParts, posePairs, threshHold,
        modelObjPath, vertexCorePath, fragmentCorePath, modelSize);
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