#include "Gui.hpp"

// Methods
Gui::Gui(const std::shared_ptr<Config>& config) : GuiConfig(config)
{
    Net = PoseEstimation::CreateDnnNet(config->ProtoTextPath, config->CaffeModel, config->DnnMode);

    FrontCamera = std::make_unique<Camera>(config->FrontCameraLinker, config->FrontCameraSize, config->CameraApi);
    BackCamera = std::make_unique<Camera>(config->BackCameraLinker, config->BackCameraSize, config->CameraApi);
    FrontCameraEstimator = std::make_unique<PoseEstimation>(Net, config->FrontCameraSize, config->PoseParts, config->PosePairs, config->ThreshHold);
    BackCameraEstimator = std::make_unique<PoseEstimation>(Net, config->BackCameraSize, config->PoseParts, config->PosePairs, config->ThreshHold);

    Model3d = std::make_unique<Model>(config->ModelObjPath, config->WindowSize);
}

// Before GUI render loop
void Gui::Init() const
{
    if (!FrontCamera->OpenCamera())
        throw std::exception("Front camera error");
    if (!BackCamera->OpenCamera())
        throw std::exception("Back camera error");

    FrontCamera->SetUpdateCameraThread(FrontCameraUpdateThread);
    BackCamera->SetUpdateCameraThread(BackCameraUpdateThread);
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
        const auto size = ImGui::GetWindowSize();

	    if(!ShowPoseEstimation)
	    {
            Model3d->Update(size);
            ImGui::Image(Model3d->GetTexture(), size);
	    }
        else
        {
	        
        }

		ImGui::End();
    }
}

static void GlfwErrorCallback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void Gui::Render() const
{
    glfwSetErrorCallback(GlfwErrorCallback);
    if (!glfwInit())
        throw std::exception("GLFW init failed");

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(static_cast<int>(GuiConfig->WindowSize.x), 
        static_cast<int>(GuiConfig->WindowSize.y), GuiConfig->Title, nullptr, nullptr);
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
    ImGui_ImplOpenGL3_Init(GuiConfig->GlslVersion);

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
