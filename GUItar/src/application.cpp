#include <guitar/application.hpp>

guitar::Application::Application(const int argc, const char** argv)
    : m_Resources(argv[0])
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
    glfwSetWindowShouldClose(m_Handle, GLFW_TRUE);
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
    m_Layout = &m_Resources.GetLayout(id);
}

void guitar::Application::SetFullscreen(const bool mode)
{
    if (mode == m_Fullscreen)
        return;

    assert(!m_InFrame);

    if (mode)
    {
        glfwGetWindowPos(m_Handle, &m_SavedState.X, &m_SavedState.Y);
        glfwGetWindowSize(m_Handle, &m_SavedState.Width, &m_SavedState.Height);

        const auto monitor = glfwGetPrimaryMonitor();
        const auto vidMode = glfwGetVideoMode(monitor);

        int x, y;
        glfwGetMonitorPos(monitor, &x, &y);

        glfwSetWindowMonitor(m_Handle, monitor, x, y, vidMode->width, vidMode->height, vidMode->refreshRate);
    }
    else
    {
        glfwSetWindowMonitor(m_Handle, nullptr, m_SavedState.X, m_SavedState.Y, m_SavedState.Width, m_SavedState.Height, GLFW_DONT_CARE);
    }

    m_Fullscreen = mode;
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
