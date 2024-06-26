#include <guitar/application.hpp>

guitar::Application::Application(const int argc, const char** ppArgv)
    : m_Resources(ppArgv[0])
{
    (void)argc;
}

void guitar::Application::Launch()
{
    Init();
    Loop();
    Destroy();
}

void guitar::Application::Close() const
{
    glfwSetWindowShouldClose(m_PHandle, GLFW_TRUE);
}

void guitar::Application::OnKey(const int key, const int scancode, const int action, const int mods)
{
    m_Events.Invoke("on_key", new KeyPayload(this, key, scancode, action, mods));
}

void guitar::Application::OnSize(const int width, const int height)
{
    m_Events.Invoke("on_size", new SizePayload(this, width, height));
}

void guitar::Application::Schedule(const ScheduleTask& task)
{
    m_Tasks.push_back(task);
}

void guitar::Application::UseLayout(const std::string& id)
{
    assert(!m_InFrame);
    m_PLayout = &m_Resources.GetLayout(id);
}

void guitar::Application::SetFullscreen(const bool active)
{
    if (active == m_Fullscreen)
        return;

    assert(!m_InFrame);

    if (active)
    {
        glfwGetWindowPos(m_PHandle, &m_SavedState.X, &m_SavedState.Y);
        glfwGetWindowSize(m_PHandle, &m_SavedState.Width, &m_SavedState.Height);

        const auto monitor = glfwGetPrimaryMonitor();
        const auto vidMode = glfwGetVideoMode(monitor);

        int x, y;
        glfwGetMonitorPos(monitor, &x, &y);

        glfwSetWindowMonitor(m_PHandle, monitor, x, y, vidMode->width, vidMode->height, vidMode->refreshRate);
    }
    else
    {
        glfwSetWindowMonitor(m_PHandle, nullptr, m_SavedState.X, m_SavedState.Y, m_SavedState.Width, m_SavedState.Height, GLFW_DONT_CARE);
    }

    m_Fullscreen = active;
}

void guitar::Application::ToggleFullscreen()
{
    SetFullscreen(!m_Fullscreen);
}

guitar::ResourceManager& guitar::Application::Resources()
{
    return m_Resources;
}

guitar::EventManager& guitar::Application::Events()
{
    return m_Events;
}
