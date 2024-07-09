#include <guitar/application.hpp>

guitar::Application::Application(const int /*argc*/, const char** ppArgv)
    : m_Resources(ppArgv[0])
{
}

void guitar::Application::Launch()
{
    Init();
    Loop();
    Destroy();
}

bool guitar::Application::IsActive() const
{
    return m_Handle && !glfwWindowShouldClose(m_Handle);
}

void guitar::Application::Close() const
{
    glfwSetWindowShouldClose(m_Handle, GLFW_TRUE);
}

void guitar::Application::OnKey(const int key, const int scancode, const int action, const int mods)
{
    const ImmutableEvent event(this, KeyPayload{key, scancode, action, mods});
    m_Events.Invoke("on_key", &event);
}

void guitar::Application::OnSize(const int width, const int height)
{
    const ImmutableEvent event(this, SizePayload{width, height});
    m_Events.Invoke("on_size", &event);
}

void guitar::Application::Schedule(const ScheduleTask& task)
{
    m_Tasks.push_back(task);
}

void guitar::Application::UseLayout(const std::string& id)
{
    assert(!m_InFrame);

    const std::filesystem::path iniDirectory = m_Resources.GetConfig().IniDirectory;
    create_directories(iniDirectory);

    if (m_Layout)
    {
        const auto iniPath = iniDirectory / m_Layout->ID;
        ImGui::SaveIniSettingsToDisk(iniPath.string().c_str());
        m_Layout->Release(m_Resources, m_Events);
    }

    m_Layout = m_Resources.GetLayout(id);

    if (m_Layout)
    {
        const auto iniPath = iniDirectory / m_Layout->ID;
        ImGui::LoadIniSettingsFromDisk(iniPath.string().c_str());
        m_Layout->Register(m_Resources, m_Events);
    }
}

void guitar::Application::ToggleFullscreen()
{
    if (m_State)
    {
        glfwSetWindowMonitor(m_Handle, nullptr, m_State->X, m_State->Y, m_State->Width, m_State->Height,
                             GLFW_DONT_CARE);
#ifdef _WIN32
        glfwSetWindowAttrib(m_Handle, GLFW_RESIZABLE, GLFW_TRUE);
#endif

        delete m_State;
        m_State = nullptr;
    }
    else
    {
        m_State = new WindowState();
        glfwGetWindowPos(m_Handle, &m_State->X, &m_State->Y);
        glfwGetWindowSize(m_Handle, &m_State->Width, &m_State->Height);

        const auto pMonitor = glfwGetPrimaryMonitor();
        const auto pMode = glfwGetVideoMode(pMonitor);
#ifdef _WIN32
        glfwSetWindowMonitor(m_Handle, nullptr, 0, 0, pMode->width, pMode->height, pMode->refreshRate);
        glfwSetWindowAttrib(m_Handle, GLFW_RESIZABLE, GLFW_TRUE);
#else
        glfwSetWindowMonitor(m_Handle, pMonitor, 0, 0, pMode->width, pMode->height, pMode->refreshRate);
#endif
    }
}

guitar::ResourceManager& guitar::Application::Resources()
{
    return m_Resources;
}

guitar::EventManager& guitar::Application::Events()
{
    return m_Events;
}

guitar::InputManager& guitar::Application::Input()
{
    return m_Input;
}
