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
    return m_PHandle && !glfwWindowShouldClose(m_PHandle);
}

void guitar::Application::Close() const
{
    glfwSetWindowShouldClose(m_PHandle, GLFW_TRUE);
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

    if (m_PLayout)
    {
        const auto iniPath = iniDirectory / m_PLayout->ID;
        ImGui::SaveIniSettingsToDisk(iniPath.string().c_str());
        m_PLayout->Release(m_Resources, m_Events);
    }

    m_PLayout = m_Resources.GetLayout(id);

    if (m_PLayout)
    {
        const auto iniPath = iniDirectory / m_PLayout->ID;
        ImGui::LoadIniSettingsFromDisk(iniPath.string().c_str());
        m_PLayout->Register(m_Resources, m_Events);
    }
}

void guitar::Application::ToggleFullscreen()
{
    if (m_PState)
    {
        glfwSetWindowMonitor(m_PHandle, nullptr, m_PState->X, m_PState->Y, m_PState->Width, m_PState->Height, GLFW_DONT_CARE);
        glfwSetWindowAttrib(m_PHandle, GLFW_RESIZABLE, GLFW_TRUE);

        delete m_PState;
        m_PState = nullptr;
    }
    else
    {
        m_PState = new WindowState();
        glfwGetWindowPos(m_PHandle, &m_PState->X, &m_PState->Y);
        glfwGetWindowSize(m_PHandle, &m_PState->Width, &m_PState->Height);

        const auto pMonitor = glfwGetPrimaryMonitor();
        const auto pMode = glfwGetVideoMode(pMonitor);
        glfwSetWindowMonitor(m_PHandle, nullptr, 0, 0, pMode->width, pMode->height, pMode->refreshRate);
        glfwSetWindowAttrib(m_PHandle, GLFW_RESIZABLE, GLFW_FALSE);
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
