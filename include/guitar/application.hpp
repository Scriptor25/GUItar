#pragma once

#define GLFW_INCLUDE_NONE

#include <functional>
#include <GLFW/glfw3.h>
#include <guitar/guitar.hpp>
#include <guitar/resources.hpp>
#include <string>

namespace guitar
{
    typedef std::function<void(int key, int scancode, int action, int mods)> KeyCallback;
    typedef std::function<void(int width, int height)> SizeCallback;

    class Application
    {
    public:
        Application(const std::filesystem::path &root);
        virtual ~Application() = default;

        bool Launch();
        void Close();

        void OnKey(int key, int scancode, int action, int mods);
        void OnSize(int width, int height);

    protected:
        virtual void OnInit(AppConfig &config) = 0;
        virtual void OnStart() = 0;
        virtual void OnImGui() = 0;
        virtual void OnStop() = 0;
        virtual void OnDestroy() = 0;

        void Register(KeyCallback callback);
        void Register(SizeCallback callback);

        void UseLayout(const std::string &id);

    private:
        bool Init();
        bool Loop();
        bool Destroy();

        GLFWwindow *m_Handle = nullptr;
        std::vector<KeyCallback> m_KeyCallbacks;
        std::vector<SizeCallback> m_SizeCallbacks;

        Resources m_Resources;
        Layout *m_Layout = nullptr;
    };

    struct AppConfig
    {
        int Width = 0;
        int Height = 0;
        std::string Title = "Application";
    };
}
