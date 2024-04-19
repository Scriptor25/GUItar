#pragma once
#define GLFW_INCLUDE_NONE

#include <functional>

#include <GLFW/glfw3.h>

#include <guitar/guitar.hpp>
#include <guitar/resources.hpp>

namespace guitar
{
    typedef std::function<void(int key, int scancode, int action, int mods)> KeyCallback;
    typedef std::function<void(int width, int height)> SizeCallback;

    class Application
    {
    public:
        explicit Application(const std::filesystem::path &executable);

        virtual ~Application() = default;

        bool Launch();

        void Close();

        void OnKey(int key, int scancode, int action, int mods);

        void OnSize(int width, int height);

    protected:
        virtual void OnInit(AppConfig &config);

        virtual void OnStart();

        virtual void OnImGui();

        virtual void OnStop();

        virtual void OnDestroy();

        void Register(const KeyCallback &callback);

        void Register(const SizeCallback &callback);

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
}
