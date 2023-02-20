#include "Gui.hpp"

// Variables
const char* Gui::GlslVersion = "#version 430 core";
const char* Gui::Title = "Pose estimation";
const ImVec2 Gui::WindowSize = ImVec2(1920, 1080);
const ImVec2 Gui::FrontCameraSize = ImVec2(640, 480);
const ImVec2 Gui::BackCameraSize = ImVec2(640, 480);
const cv::VideoCaptureAPIs Gui::CameraApi = cv::CAP_ANY;
const std::string Gui::ProtoTextPath = R"(\models\pose\coco\pose_deploy_linevec.prototxt)";
const std::string Gui::CaffeModel = R"(\models\pose\coco\pose_iter_440000.caffemodel)";
bool Gui::ShowPoseEstimation = false;
std::unique_ptr<Camera> Gui::FrontCamera = std::make_unique<Camera>(0, FrontCameraSize, CameraApi);
std::unique_ptr<Camera> Gui::BackCamera = std::make_unique<Camera>("http://192.168.1.108:4747/video", BackCameraSize, CameraApi);
std::unique_ptr<std::jthread> Gui::FrontCameraUpdateThread = std::make_unique<std::jthread>();
std::unique_ptr<std::jthread> Gui::BackCameraUpdateThread = std::make_unique<std::jthread>();
std::unique_ptr<PoseEstimation> Gui::FrontPoseEstimation = std::make_unique<PoseEstimation>(ProtoTextPath, CaffeModel);
//std::unique_ptr<PoseEstimation> Gui::BackPoseEstimation = std::make_unique<PoseEstimation>(ProtoTextPath, CaffeModel);

// Methods
// Before GUI render loop
void Gui::Init()
{
    if (!FrontCamera->OpenCamera())
        throw std::exception("Front camera error");
    if (!BackCamera->OpenCamera())
        throw std::exception("Back camera error");

    FrontCamera->SetUpdateCameraThread(FrontCameraUpdateThread);
    BackCamera->SetUpdateCameraThread(BackCameraUpdateThread);

    FrontPoseEstimation->Create(FrontCamera->GetMat());
    //BackPoseEstimation->Create(BackCamera->GetMat());
}

// Inside GUI render loop
void Gui::Loop()
{
    //ImGui::ShowDemoWindow();

    // Debug window
	ImGui::Text("%.1f FPS", static_cast<double>(ImGui::GetIO().Framerate));
    ImGui::Spacing();
    ImGui::Checkbox("Show pose estimation", &ShowPoseEstimation);

    // Front camera window
	ImGui::Begin("Front camera");
    FrontCamera->UpdateImage();
    if (ShowPoseEstimation)
    {
        FrontPoseEstimation->Update(FrontCamera->GetMat());
	    /*ImGui::Image(FrontPoseEstimation->GetTexture(), FrontCameraSize);*/
    }
    else    
    {        
	    ImGui::Image(FrontCamera->GetTexture(), FrontCameraSize);
    }
    ImGui::End();

    // Back camera window
    ImGui::Begin("Back camera");
    BackCamera->UpdateImage();
    if(ShowPoseEstimation)
    {
        //BackPoseEstimation->Update(BackCamera->GetMat());
		/*ImGui::Image(BackPoseEstimation->GetTexture(), BackCameraSize);*/
    }
    else
    {        
		ImGui::Image(BackCamera->GetTexture(), BackCameraSize);
    }
    ImGui::End();
}

static void GlfwErrorCallback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void Gui::Render()
{
    glfwSetErrorCallback(GlfwErrorCallback);
    if (!glfwInit())
        throw std::exception("GLFW init failed");

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(static_cast<int>(WindowSize.x), static_cast<int>(WindowSize.y), Title, nullptr, nullptr);
    if (window == nullptr)
        throw std::exception("Gui window error");
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
	(void)io;

    // Setup ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(GlslVersion);

    constexpr auto clearColor = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);

    // Init
    Init();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Interface
        Loop();

        // Rendering
        ImGui::Render();
        int displayW, displayH;
        glfwGetFramebufferSize(window, &displayW, &displayH);
        glViewport(0, 0, displayW, displayH);
        glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}