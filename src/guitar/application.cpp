#include <GL/glew.h>
#include <guitar/application.hpp>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>
#include <iostream>

guitar::Application::Application(const std::filesystem::path &root)
    : m_Resources(root)
{
}

bool guitar::Application::Launch()
{
    if (Init())
        return true;
    if (Loop())
        return true;
    if (Destroy())
        return true;

    return true;
}

void guitar::Application::Close()
{
    glfwSetWindowShouldClose(m_Handle, GLFW_TRUE);
}

void guitar::Application::OnKey(int key, int scancode, int action, int mods)
{
    for (auto &callback : m_KeyCallbacks)
        callback(key, scancode, action, mods);
}

void guitar::Application::OnSize(int width, int height)
{
    for (auto &callback : m_SizeCallbacks)
        callback(width, height);
}

static void glfw_error_callback(int error_code, const char *description)
{
    std::cerr << "[GLFW 0x" << std::hex << error_code << std::dec << "] " << description << std::endl;
}

void guitar::Application::Register(KeyCallback callback)
{
    m_KeyCallbacks.push_back(callback);
}

void guitar::Application::Register(SizeCallback callback)
{
    m_SizeCallbacks.push_back(callback);
}

static void glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    auto &app = *((guitar::Application *)glfwGetWindowUserPointer(window));
    app.OnKey(key, scancode, action, mods);
}

static void glfw_size_callback(GLFWwindow *window, int width, int height)
{
    auto &app = *((guitar::Application *)glfwGetWindowUserPointer(window));
    app.OnSize(width, height);
}

bool guitar::Application::Init()
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        std::cerr << "[GUItar] Failed to init glfw" << std::endl;
        return true;
    }

    AppConfig config;
    OnInit(config);

    if (config.Width == 0 || config.Height == 0)
    {
        auto vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        if (config.Width == 0)
            config.Width = vidmode->width / 2;
        if (config.Height == 0)
            config.Height = vidmode->height / 2;
    }

    m_Handle = glfwCreateWindow(config.Width, config.Height, config.Title.c_str(), nullptr, nullptr);
    if (!m_Handle)
    {
        std::cerr << "[GUItar] Failed to create glfw window" << std::endl;
        return true;
    }

    glfwSetWindowUserPointer(m_Handle, this);
    glfwSetKeyCallback(m_Handle, glfw_key_callback);
    glfwSetWindowSizeCallback(m_Handle, glfw_size_callback);

    glfwMakeContextCurrent(m_Handle);
    glfwSwapInterval(1);

    if (int err = glewInit())
    {
        auto msg = glewGetErrorString(err);
        std::cerr << "[GLEW 0x" << std::hex << err << std::dec << "] " << msg << std::endl;
        std::cerr << "[GUItar] Failed to init glew" << std::endl;
        return true;
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

    return 0;
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

    return 0;
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

    return 0;
}
