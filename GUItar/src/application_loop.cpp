#define GLFW_INCLUDE_NONE

#include <implot.h>
#include <iostream>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <guitar/application.hpp>
#include <guitar/layout.hpp>

static guitar::Application& get_app(GLFWwindow* window)
{
    return *static_cast<guitar::Application*>(glfwGetWindowUserPointer(window));
}

static void on_error(const int error_code, const char* description)
{
    std::cerr << "[GLFW 0x" << std::hex << error_code << std::dec << "] " << description << std::endl;
}

static void on_key(GLFWwindow* window, const int key, const int scancode, const int action, const int mods)
{
    auto& app = get_app(window);
    app.OnKey(key, scancode, action, mods);
}

static void on_size(GLFWwindow* window, const int width, const int height)
{
    auto& app = get_app(window);
    app.OnSize(width, height);
}

void guitar::Application::Init()
{
    m_Resources.Index();

    glfwSetErrorCallback(on_error);
    if (glfwInit() != GLFW_TRUE)
    {
        std::cerr << "[Application] Failed to init glfw" << std::endl;
        throw std::runtime_error("failed to init glfw");
    }

    auto& config = m_Resources.GetConfig();
    OnInit(config);

    if (config.Width <= 0 || config.Height <= 0)
    {
        const auto vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        if (config.Width == 0)
            config.Width = vidMode->width / 2;
        if (config.Height == 0)
            config.Height = vidMode->height / 2;
    }

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_SAMPLES, 4);
    m_Handle = glfwCreateWindow(config.Width, config.Height, config.Title.c_str(), nullptr, nullptr);
    if (!m_Handle)
    {
        std::cerr << "[Application] Failed to create glfw window" << std::endl;
        throw std::runtime_error("failed to create glfw window");
    }

    glfwSetWindowUserPointer(m_Handle, this);
    glfwSetKeyCallback(m_Handle, on_key);
    glfwSetWindowSizeCallback(m_Handle, on_size);

    glfwMakeContextCurrent(m_Handle);
    glfwSwapInterval(1);

    if (const auto err = glewInit())
    {
        std::cerr << "[Application] Failed to init glew:" << std::endl;
        const auto msg = glewGetErrorString(err);
        std::cerr << "[GLEW 0x" << std::hex << err << std::dec << "] " << msg << std::endl;
        throw std::runtime_error("failed to init glew");
    }

    glEnable(GL_MULTISAMPLE);

    m_Resources.LoadAllImages();

    ImGui::CreateContext();
    ImPlot::CreateContext();

    auto& io = ImGui::GetIO();
    if (config.Viewports)
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigDockingTransparentPayload = true;
    io.ConfigViewportsNoDecoration = true;
    io.ConfigViewportsNoTaskBarIcon = true;
    io.IniFilename = nullptr;

    ImGui_ImplGlfw_InitForOpenGL(m_Handle, true);
    ImGui_ImplOpenGL3_Init();

    UseLayout(config.Layout);
}

void guitar::Application::Loop()
{
    OnStart();

    while (!glfwWindowShouldClose(m_Handle))
    {
        for (const auto& task : m_Tasks)
            task();
        m_Tasks.clear();

        // Update Input Manager
        m_Input.Update(m_Handle);

        OnFrame();

        m_InFrame = true;

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        if (m_Layout)
            m_Layout->Draw(m_Resources, m_Events);

        OnImGui();

        ImGui::Render();

        int width, height;
        glfwGetFramebufferSize(m_Handle, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(m_Handle);
        }

        m_InFrame = false;

        glfwSwapBuffers(m_Handle);
        glfwPollEvents();
    }

    OnStop();
}

void guitar::Application::Destroy()
{
    OnDestroy();

    UseLayout({});

    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();

    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_Handle);
    glfwTerminate();

    m_Handle = nullptr;
}
