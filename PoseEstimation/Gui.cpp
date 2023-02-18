#include "Gui.h"

// Variables
const char* Gui::GlslVersion = "#version 330";
const char* Gui::Title = "Pose estimation";
const ImVec2 Gui::WindowSize = ImVec2(1920, 1080);
const ImVec2 Gui::FrontCameraSize = ImVec2(640, 480);
const ImVec2 Gui::BackCameraSize = ImVec2(640, 480);
const cv::VideoCaptureAPIs Gui::CameraApi = cv::CAP_ANY;
std::unique_ptr<PoseEstimation> Gui::FrontCamera = std::make_unique<PoseEstimation>(1, FrontCameraSize, CameraApi);
std::unique_ptr<PoseEstimation> Gui::BackCamera = std::make_unique<PoseEstimation>("http://192.168.1.108:4747/video", BackCameraSize, CameraApi);
std::unique_ptr<std::jthread> Gui::FrontCameraUpdateThread = std::make_unique<std::jthread>();
std::unique_ptr<std::jthread> Gui::BackCameraUpdateThread = std::make_unique<std::jthread>();

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
}

// Inside GUI render loop
void Gui::Loop()
{
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0 / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Begin("Camera");

    FrontCamera->UpdateImage();
    BackCamera->UpdateImage();

    ImGui::Image(FrontCamera->GetTexture(), FrontCameraSize);
    ImGui::Image(BackCamera->GetTexture(), BackCameraSize);

    //imshow("Front", FrontCamera->Image->Mat);
    //imshow("Back", BackCamera->Image->Mat);

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
    //glfwSwapInterval(1);

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
