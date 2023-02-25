#include "Gui.hpp"

// Methods
Gui::Gui(const std::shared_ptr<Config>& config) : GuiConfig(config)
{
    InitGlfw();
    Window = CreateWindow();
    Net = PoseEstimation::CreateDnnNet(config->ProtoTextPath, config->CaffeModel, config->DnnMode);

    FrontCamera = std::make_unique<Camera>(config->FrontCameraLinker, config->FrontCameraSize, config->CameraApi);
    BackCamera = std::make_unique<Camera>(config->BackCameraLinker, config->BackCameraSize, config->CameraApi);
    FrontCameraEstimator = std::make_unique<PoseEstimation>(Net, config->FrontCameraSize, config->PoseParts, config->PosePairs, config->ThreshHold);
    BackCameraEstimator = std::make_unique<PoseEstimation>(Net, config->BackCameraSize, config->PoseParts, config->PosePairs, config->ThreshHold);

    Model3d = std::make_unique<Model>(config->ModelObjPath, config->WindowSize);
}

// Inside GUI render loop
void Gui::Loop() const
{
    //ImGui::ShowDemoWindow();

    // Debug window
	ImGui::Text("%.1f FPS", static_cast<double>(ImGui::GetIO().Framerate));
    ImGui::Spacing();
    ImGui::Checkbox("Pose estimation", &ShowPoseEstimation);
    ImGui::Checkbox("3D model", &Show3dModel);

    // Front camera window
	ImGui::Begin("Front camera");
    if (ShowPoseEstimation)
    {
        FrontCameraEstimator->Update(FrontCamera->GetMat());
	    ImGui::Image(FrontCameraEstimator->GetTexture(), GuiConfig->FrontCameraSize);
    }
    else    
    {        
		FrontCamera->UpdateImage(); 
	    ImGui::Image(FrontCamera->GetTexture(), GuiConfig->FrontCameraSize);
    }
    ImGui::End();

	// Back camera window
    ImGui::Begin("Back camera");
    if(ShowPoseEstimation)
    {
        BackCameraEstimator->Update(BackCamera->GetMat());
		ImGui::Image(BackCameraEstimator->GetTexture(), GuiConfig->BackCameraSize);
    }
    else
    {        
		BackCamera->UpdateImage();
		ImGui::Image(BackCamera->GetTexture(), GuiConfig->BackCameraSize);
    }
    ImGui::End();

    // 3d model window
    if(Show3dModel)
    {
		ImGui::Begin("3D model");
        const auto windowDrawList = ImGui::GetWindowDrawList();

	    if(!ShowPoseEstimation)
	    {
           // windowDrawList->AddImage(ImVec2(ImGui::GetCursorScreenPos()),
           //     ImVec2(ImGui::GetCursorScreenPos().x + window.getWidth() / 2, ImGui::GetCursorScreenPos().y + window.getHeight() / 2), ImVec2(0, 1), ImVec2(1, 0))
	    }
        else
        {
	        
        }

		ImGui::End();
    }
}

void Gui::Render() const
{
    glfwMakeContextCurrent(Window);
    glfwSwapInterval(1);

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
	(void)io;

    // Setup ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(Window, true);
    ImGui_ImplOpenGL3_Init(GuiConfig->GlslVersion);

    constexpr auto clearColor = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);

    // Setup cameras
    SetupCamera();

    while (!glfwWindowShouldClose(Window))
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
        glfwGetFramebufferSize(Window, &displayW, &displayH);
        glViewport(0, 0, displayW, displayH);
        glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(Window);
    }
}

GLFWwindow* Gui::CreateWindow() const
{
    GLFWwindow* window = glfwCreateWindow(static_cast<int>(GuiConfig->WindowSize.x),
        static_cast<int>(GuiConfig->WindowSize.y), GuiConfig->Title, nullptr, nullptr);

    if (window == nullptr)
        throw std::exception("Gui window error");

    return window;
}

static void GlfwErrorCallback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void Gui::InitGlfw() const
{
    glfwSetErrorCallback(GlfwErrorCallback);
    if (!glfwInit())
        throw std::exception("GLFW init failed");
}

void Gui::SetupCamera() const
{
    if (!FrontCamera->OpenCamera())
        throw std::exception("Front camera error");
    if (!BackCamera->OpenCamera())
        throw std::exception("Back camera error");

    FrontCamera->SetUpdateCameraThread(FrontCameraUpdateThread);
    BackCamera->SetUpdateCameraThread(BackCameraUpdateThread);
}

Gui::~Gui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(Window);
    glfwTerminate();
}