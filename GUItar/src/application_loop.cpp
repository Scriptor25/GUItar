#include <iostream>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <guitar/application.hpp>
#include <guitar/layout.hpp>

static void glfw_error_callback(const int error_code, const char* description)
{
    std::cerr << "[GLFW 0x" << std::hex << error_code << std::dec << "] " << description << std::endl;
}

static void glfw_key_callback(GLFWwindow* window, const int key, const int scancode, const int action, const int mods)
{
    auto& app = *static_cast<guitar::Application*>(glfwGetWindowUserPointer(window));
    app.OnKey(key, scancode, action, mods);
}

static void glfw_size_callback(GLFWwindow* window, const int width, const int height)
{
    auto& app = *static_cast<guitar::Application*>(glfwGetWindowUserPointer(window));
    app.OnSize(width, height);
}

void guitar::Application::Init()
{
    m_Resources.Index();

    glfwSetErrorCallback(glfw_error_callback);
    if (glfwInit() != GLFW_TRUE)
    {
        std::cerr << "[Application] Failed to init glfw" << std::endl;
        throw std::runtime_error("failed to init glfw");
    }

    auto& config = m_Resources.GetConfig();
    OnInit(config);

    if (config.Width == 0 || config.Height == 0)
    {
        const auto vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        if (config.Width == 0)
            config.Width = vidMode->width / 2;
        if (config.Height == 0)
            config.Height = vidMode->height / 2;
    }

    m_PHandle = glfwCreateWindow(config.Width, config.Height, config.Title.c_str(), nullptr, nullptr);
    if (!m_PHandle)
    {
        std::cerr << "[Application] Failed to create glfw window" << std::endl;
        throw std::runtime_error("failed to create glfw window");
    }

    glfwSetWindowUserPointer(m_PHandle, this);
    glfwSetKeyCallback(m_PHandle, glfw_key_callback);
    glfwSetWindowSizeCallback(m_PHandle, glfw_size_callback);

    glfwMakeContextCurrent(m_PHandle);
    glfwSwapInterval(1);

    if (const auto err = glewInit())
    {
        std::cerr << "[Application] Failed to init glew:" << std::endl;
        const auto msg = glewGetErrorString(err);
        std::cerr << "[GLEW 0x" << std::hex << err << std::dec << "] " << msg << std::endl;
        throw std::runtime_error("failed to init glew");
    }

    m_Resources.LoadAllImages();
    UseLayout(config.Layout);

    ImGui::CreateContext();
    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    if (config.Viewports) io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigDockingTransparentPayload = true;

    ImGui_ImplOpenGL3_Init();
    ImGui_ImplGlfw_InitForOpenGL(m_PHandle, true);
}

void guitar::Application::Loop()
{
    OnStart();

    while (!glfwWindowShouldClose(m_PHandle))
    {
        for (const auto& task : m_Tasks)
            task();
        m_Tasks.clear();

        OnFrame();

        m_InFrame = true;

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        if (m_PLayout)
            m_PLayout->Draw(m_Resources, m_Events);
        OnImGui();

        ImGui::Render();

        int width, height;
        glfwGetFramebufferSize(m_PHandle, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(m_PHandle);
        }

        glfwSwapBuffers(m_PHandle);
        glfwPollEvents();

        m_InFrame = false;
    }

    OnStop();
}

void guitar::Application::Destroy()
{
    OnDestroy();

    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_PHandle);
    m_PHandle = nullptr;
    glfwTerminate();
}
