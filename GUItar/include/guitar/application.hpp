#pragma once
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>

#include <functional>
#include <guitar/events.hpp>
#include <guitar/guitar.hpp>
#include <guitar/resources.hpp>

namespace guitar
{
    struct WindowState
    {
        int X = 0;
        int Y = 0;
        int Width = 0;
        int Height = 0;
    };

    class Application
    {
    public:
        explicit Application(const std::filesystem::path &executable);

        virtual ~Application() = default;

        bool Launch();

        void Close() const;

        void OnKey(int key, int scancode, int action, int mods);

        void OnSize(int width, int height);

    protected:
        virtual void OnInit(AppConfig &config);

        virtual void OnStart();

        virtual void OnImGui();

        virtual void OnStop();

        virtual void OnDestroy();

        void UseLayout(const std::string &id);

        void SetFullscreen(bool mode);

        void ToggleFullscreen();

        ResourceManager &Resources();

        EventManager &Events();

    private:
        bool Init();

        bool Loop();

        bool Destroy();

        GLFWwindow *m_Handle = nullptr;

        WindowState m_SavedState{};
        bool m_Fullscreen = false;

        ResourceManager m_Resources;
        EventManager m_Events;

        Layout *m_Layout = nullptr;
    };
}
