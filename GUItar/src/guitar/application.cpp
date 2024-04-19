#include <iostream>

#include <GL/glew.h>

#include <guitar/application.hpp>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

guitar::Application::Application(const std::filesystem::path &executable)
        : m_Resources(executable)
{
}

bool guitar::Application::Launch()
{
    return Init() && Loop() && Destroy();
}

void guitar::Application::Close()
{
    glfwSetWindowShouldClose(m_Handle, GLFW_TRUE);
}

void guitar::Application::OnKey(int key, int scancode, int action, int mods)
{
    for (auto &callback: m_KeyCallbacks)
        callback(key, scancode, action, mods);
}

void guitar::Application::OnSize(int width, int height)
{
    for (auto &callback: m_SizeCallbacks)
        callback(width, height);
}

void guitar::Application::OnInit(AppConfig &config)
{
}

void guitar::Application::OnStart()
{
}

void guitar::Application::OnImGui()
{
}

void guitar::Application::OnStop()
{
}

void guitar::Application::OnDestroy()
{
}

static void glfw_error_callback(int error_code, const char *description)
{
    std::cerr << "[GLFW 0x" << std::hex << error_code << std::dec << "] " << description << std::endl;
}

void guitar::Application::Register(const KeyCallback &callback)
{
    m_KeyCallbacks.push_back(callback);
}

void guitar::Application::Register(const SizeCallback &callback)
{
    m_SizeCallbacks.push_back(callback);
}

void guitar::Application::UseLayout(const std::string &id)
{
    m_Layout = m_Resources.GetLayout(id);
}

static void glfw_key_callback(GLFWwindow *window, const int key, const int scancode, const int action, const int mods)
{
    auto &app = *static_cast<guitar::Application *>(glfwGetWindowUserPointer(window));
    app.OnKey(key, scancode, action, mods);
}

static void glfw_size_callback(GLFWwindow *window, const int width, const int height)
{
    auto &app = *static_cast<guitar::Application *>(glfwGetWindowUserPointer(window));
    app.OnSize(width, height);
}

bool guitar::Application::Init()
{
    m_Resources.Index();

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        std::cerr << "[GUItar] Failed to init glfw" << std::endl;
        return false;
    }

    auto &config = m_Resources.GetApp();
    OnInit(config);

    if (config.Width == 0 || config.Height == 0)
    {
        auto vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        if (config.Width == 0)
            config.Width = vidMode->width / 2;
        if (config.Height == 0)
            config.Height = vidMode->height / 2;
    }

    m_Handle = glfwCreateWindow(config.Width, config.Height, config.Title.c_str(), nullptr, nullptr);
    if (!m_Handle)
    {
        std::cerr << "[GUItar] Failed to create glfw window" << std::endl;
        return false;
    }

    glfwSetWindowUserPointer(m_Handle, this);
    glfwSetKeyCallback(m_Handle, glfw_key_callback);
    glfwSetWindowSizeCallback(m_Handle, glfw_size_callback);

    glfwMakeContextCurrent(m_Handle);
    glfwSwapInterval(1);

    if (const auto err = glewInit())
    {
        const auto msg = glewGetErrorString(err);
        std::cerr << "[GLEW 0x" << std::hex << err << std::dec << "] " << msg << std::endl;
        std::cerr << "[GUItar] Failed to init glew" << std::endl;
        return false;
    }

    ImGui::CreateContext();
    auto &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigDockingTransparentPayload = true;

    ImGui_ImplOpenGL3_Init();
    ImGui_ImplGlfw_InitForOpenGL(m_Handle, true);

    UseLayout(config.Layout);

    return true;
}

bool guitar::Application::Loop()
{
    OnStart();

    while (!glfwWindowShouldClose(m_Handle))
    {
        glfwPollEvents();

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        if (m_Layout)
            m_Layout->Draw();
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

        glfwSwapBuffers(m_Handle);
    }

    OnStop();

    return true;
}

bool guitar::Application::Destroy()
{
    OnDestroy();

    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_Handle);
    m_Handle = nullptr;
    glfwTerminate();

    return true;
}
