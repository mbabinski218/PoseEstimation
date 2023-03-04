#include "Gui.hpp"

// Methods
Gui::Gui(const std::shared_ptr<Config>& config) : Window(nullptr),	GuiConfig(config),
	FrontCameraCancellationToken(false), BackCameraCancellationToken(false)
{
    InitGlfw();
    InitWindow();
    InitGlew();
    InitOpenGL();    

    FrontCamera = std::make_unique<Camera>(config->FrontCameraLinker, config->FrontCameraSize, config->CameraApi);
    //BackCamera = std::make_unique<Camera>(config->BackCameraLinker, config->BackCameraSize, config->CameraApi);

    Net = PoseEstimation::CreateDnnNet(config->ProtoTextPath, config->CaffeModel, config->DnnMode);
    FrontCameraEstimator = std::make_unique<PoseEstimation>(Net, config->FrontCameraSize, config->PoseParts, config->PosePairs, config->ThreshHold);
    BackCameraEstimator = std::make_unique<PoseEstimation>(Net, config->BackCameraSize, config->PoseParts, config->PosePairs, config->ThreshHold);

    Model = std::make_unique<Mesh>(config->ModelObjPath, config->VertexCorePath, config->FragmentCorePath);

    Padding = ImGui::GetStyle().WindowPadding;

    InitCamera();
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
    ImGui::Checkbox("3D model controls", &Show3dModelControls);

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
  //  ImGui::Begin("Back camera");
  //  if(ShowPoseEstimation)
  //  {
  //      BackCameraEstimator->Update(BackCamera->GetMat());
		//ImGui::Image(BackCameraEstimator->GetTexture(), GuiConfig->BackCameraSize);
  //  }
  //  else
  //  {        
		//BackCamera->UpdateImage();
		//ImGui::Image(BackCamera->GetTexture(), GuiConfig->BackCameraSize);
  //  }
  //  ImGui::End();

    // 3d model window
    if (Show3dModel)
    {
		ImGui::Begin("3D model");

        const auto windowSize = ImGui::GetWindowSize();        
        Model->Update(windowSize);
        Model->Render();
        ImGui::Image(Model->GetTexture(), ImVec2(windowSize.x - 2.0f * Padding.x, windowSize.y - 4.5f * Padding.y));

		ImGui::End();
    }

    // 3d model controls window
    if (Show3dModelControls)
    {
        ImGui::Begin("3D model controls");

        ImGui::SliderInt("Fov", Model->FovPtr(), 20, 90);
        ImGui::SliderInt("Pitch", Model->PitchPtr(), -180, 180);
        ImGui::SliderInt("Yaw", Model->YawPtr(), -180, 180);
        ImGui::SliderInt("Roll", Model->RollPtr(), -180, 180);
        ImGui::SliderFloat("Distance", Model->DistancePtr(), 0.0f, 30.0f);
        ImGui::SliderFloat3("Focus", Model->FocusPtr(), -2.0f, 2.0f);

        ImGui::Spacing();
        if (ImGui::Button("Reset"))
            Model->Reset();

        ImGui::End();
    }
}

void Gui::HandleInput() const
{
    double x, y;
    glfwGetCursorPos(Window, &x, &y);

    // Model input
    if(Show3dModel)
		Model->OnMouseMove(x, y, Input::GetPressedButton(Window));
}

void Gui::Render() const
{
	while (!glfwWindowShouldClose(Window))
    {
        glfwPollEvents();

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Interface
        Loop();

        // Input
        HandleInput();

        // Rendering
        ImGui::Render();
        int displayW, displayH;
        glfwGetFramebufferSize(Window, &displayW, &displayH);
        glViewport(0, 0, displayW, displayH);
        glClearColor(BgColor.x * BgColor.w, BgColor.y * BgColor.w, BgColor.z * BgColor.w, BgColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(Window);
    }
}

void Gui::InitGlfw() const
{
    glfwSetErrorCallback(GlfwErrorCallback);
    if (!glfwInit())
        throw std::exception("GLFW init failed");
}

void Gui::InitWindow()
{
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_RESIZABLE, false);

    Window = glfwCreateWindow(static_cast<int>(GuiConfig->WindowSize.x), 
        static_cast<int>(GuiConfig->WindowSize.y), GuiConfig->Title, nullptr, nullptr);

    if (Window == nullptr)
        throw std::exception("Gui window init failed");

    glfwSetFramebufferSizeCallback(Window, FrameBufferResizeCallback);
    glfwMakeContextCurrent(Window);
}

void Gui::InitGlew() const
{
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
        throw std::exception("GLEW init failed");
}

void Gui::InitOpenGL() const
{
    // Vsync
    glfwSwapInterval(true);

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // Setup ImGui font
    const auto path = std::filesystem::current_path().string() + GuiConfig->FontPath;
    io.FontDefault = io.Fonts->AddFontFromFileTTF(path.c_str(), 14.0f);

    // Setup ImGui style
    ImGui::StyleColorsDark();
    ImGui::GetStyle().WindowBorderSize = 0.0f;
    ImGui::GetStyle().FrameRounding = 5.0f;
    ImGui::GetStyle().WindowRounding = 5.0f;
    ImGui::GetStyle().ItemSpacing = { 7.0f, 7.0f };

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(Window, true);
    ImGui_ImplOpenGL3_Init(GuiConfig->GlslVersion);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_DEBUG_OUTPUT);
    //glDebugMessageCallback(GlMessageCallback, nullptr);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Gui::InitCamera() const
{
    if (!FrontCamera->OpenCamera())
        throw std::exception("Front camera error");
    //if (!BackCamera->OpenCamera())
    //    throw std::exception("Back camera error");

    FrontCamera->SetUpdateCameraThread(FrontCameraUpdateThread, FrontCameraCancellationToken);
    /*BackCamera->SetUpdateCameraThread(BackCameraUpdateThread, BackCameraCancellationToken);*/
}

void Gui::ShutdownCamera()
{
    FrontCameraCancellationToken = true;
    //BackCameraCancellationToken = true;
}

Gui::~Gui()
{
    ShutdownCamera();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(Window);
    glfwTerminate();
}

void Gui::FrameBufferResizeCallback(GLFWwindow* window, int fbW, int fbH)
{
    glViewport(0, 0, fbW, fbH);
};

void Gui::GlfwErrorCallback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void Gui::GlMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam)
{
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
}