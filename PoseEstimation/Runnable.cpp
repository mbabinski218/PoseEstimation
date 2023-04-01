#include "Runnable.hpp"
#include "Config.hpp"

Runnable::Runnable() : Window(nullptr)
{
    InitGlfw();
    InitWindow();
    InitGlew();
    InitOpenGL();

    Padding = ImGui::GetStyle().WindowPadding;
}

void Runnable::Render()
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
        constexpr auto bgColor = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);
        glClearColor(bgColor.x * bgColor.w, bgColor.y * bgColor.w, bgColor.z * bgColor.w, bgColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(Window);
    }
}

void Runnable::InitGlfw() const
{
    glfwSetErrorCallback(GlfwErrorCallback);
    if (!glfwInit())
        throw std::exception("GLFW init failed");
}

void Runnable::InitWindow()
{
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_RESIZABLE, false);

    Window = glfwCreateWindow(static_cast<int>(Config::WindowSize.x),
        static_cast<int>(Config::WindowSize.y), Config::Title, nullptr, nullptr);

    if (Window == nullptr)
        throw std::exception("Gui window init failed");

    glfwSetFramebufferSizeCallback(Window, FrameBufferResizeCallback);
    glfwMakeContextCurrent(Window);
}

void Runnable::InitGlew() const
{
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
        throw std::exception("GLEW init failed");
}

void Runnable::InitOpenGL() const
{
    // Vsync
    glfwSwapInterval(true);

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // Setup ImGui font
    const auto path = std::filesystem::current_path().string() + Config::FontPath;
    io.FontDefault = io.Fonts->AddFontFromFileTTF(path.c_str(), 14.0f);

    // Setup ImGui style
    ImGui::StyleColorsDark();
    ImGui::GetStyle().WindowBorderSize = 0.0f;
    ImGui::GetStyle().FrameRounding = 5.0f;
    ImGui::GetStyle().WindowRounding = 5.0f;
    ImGui::GetStyle().ItemSpacing = { 7.0f, 7.0f };

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(Window, true);
    ImGui_ImplOpenGL3_Init(Config::GlslVersion);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Runnable::Shutdown() const
{
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

void Runnable::FrameBufferResizeCallback(GLFWwindow* window, int fbW, int fbH)
{
    glViewport(0, 0, fbW, fbH);
}

void Runnable::GlfwErrorCallback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void Runnable::GlMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam)
{
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
}