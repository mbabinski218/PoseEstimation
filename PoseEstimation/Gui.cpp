#include "Gui.h"

// Variables
const char* Gui::GlslVersion = "#version 330";
const char* Gui::Title = "Pose estimation";
const int Gui::Width = 1920;
const int Gui::Height = 1080;
PoseEstimation* Gui::FrontCamera = new PoseEstimation(0, 1280, 720);
std::unique_ptr<PoseEstimation> Gui::BackCamera = std::make_unique<PoseEstimation>(1, 1280, 720);

// Methods
void Gui::Init()
{
    if (!FrontCamera->OpenCamera())
        throw std::exception("Front camera error");

    //if (!BackCamera->OpenCamera())
    //    throw std::exception("Back camera error");
}

void Gui::Loop()
{
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0 / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Begin("Camera");

    std::jthread j1(&PoseEstimation::UpdateImage, FrontCamera);
    //std::jthread j2(&PoseEstimation::UpdateImage, BackCamera);
    j1.join();

    ImGui::Image(FrontCamera->Image->GetTexture(), *FrontCamera->Image->GetSize());
    //ImGui::Image(BackCamera->Image->GetTexture(), *BackCamera->Image->GetSize());

    //imshow("Frame", *Camera->Frame);

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
    GLFWwindow* window = glfwCreateWindow(Width, Height, Title, nullptr, nullptr);
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
