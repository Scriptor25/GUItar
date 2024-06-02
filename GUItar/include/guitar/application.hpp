#pragma once
#define GLFW_INCLUDE_NONE

#include <functional>
#include <GLFW/glfw3.h>
#include <guitar/events.hpp>
#include <guitar/guitar.hpp>
#include <guitar/resources.hpp>

namespace guitar
{
    /**
     * The saved window position and size,
     * used for recreating the window after
     * switching from fullscreen to windowed mode.
     * Internal usage only.
     */
    struct WindowState
    {
        int X = 0;
        int Y = 0;
        int Width = 0;
        int Height = 0;
    };

    /**
     * A simple callback for scheduling tasks.
     * Internal usage only.
     */
    typedef std::function<void()> ScheduleTask;

    /**
     * The application base class serves as an extendable class.
     * To use it simply extend it publicly with your custom class
     * and transfer argc and argv from your main function via the
     * constructor.
     *
     * With a class extending Application you can access functions
     * like @code ToggleFullscreen()@endcode to switch between
     * windowed and fullscreen mode, or @code UseLayout(id)@endcode
     * to set an active layout.
     *
     * You can also access the resources and event managers. On startup
     * the application creates both. The resources manager is responsible
     * for indexing the resources, like layouts, app configs etc. The
     * event manager lets you register listeners for events, indexed
     * by a string id. To register a key listener, use
     * @code Events().Register("on_key", [](const guitar::EventPayload* payload) { ... })@endcode.
     * This will give you access to an event payload pointer. You have to cast
     * it to the required payload yourself, e.g. for a key listener it is a
     * @code KeyPayload@endcode. All of this is nearly exactly the same for
     * a window size listener, except you have to register on @code "on_size"@endcode
     * and cast to a @code SizePayload@endcode. For custom events, e.g. from buttons
     * you just use a custom name, and the payload does not need to be casted as it
     * only contains a pointer to the source, i.e. the button.
     *
     * You can also override some event functions like @code OnInit(config)@endcode,
     * @code OnStart()@endcode, @code OnImGui()@endcode etc. But usually if you only want to
     * use xml for config and layouts you don't need them.
     */
    class Application
    {
    public:
        explicit Application(int argc, const char** ppArgv);
        virtual ~Application() = default;

        /**
         * Launch the application.
         */
        void Launch();

        /**
         * Close the application.
         */
        void Close() const;

        /**
         * Gets called whenever a key is pressed. Internal usage only.
         *
         * @param key the key code
         * @param scancode the scancode
         * @param action either PRESS, RELEASE or REPEAT
         * @param mods flags if alt, ctrl or other mod keys are simultaneously pressed.
         */
        void OnKey(int key, int scancode, int action, int mods);

        /**
         * Gets called whenever the window size changes. Internal usage only.
         *
         * @param width the new width
         * @param height the new height
         */
        void OnSize(int width, int height);

    protected:
        /**
         * This gets called even before the window gets created.
         * You can set some parameters like window size, title etc.
         *
         * @param config the application config
         */
        virtual void OnInit(AppConfig& config);

        /**
         * This gets called before the first frame.
         */
        virtual void OnStart();

        /**
         * This gets called on every frame start.
         */
        virtual void OnFrame();

        /**
         * This gets called every frame when ImGui expects input
         * to be submitted.
         */
        virtual void OnImGui();

        /**
         * This gets called after the window closes.
         */
        virtual void OnStop();

        /**
         * This gets called when the application performs
         * cleanup actions.
         */
        virtual void OnDestroy();

        /**
         * Executes the given task at the beginning of the
         * next frame.
         *
         * @param task the task to execute
         */
        void Schedule(const ScheduleTask& task);

        /**
         * Sets the active layout to the one with the given id.
         * This doesn't throw any error if the id does not exist,
         * it just doesn't draw anything.
         *
         * @param id the layout id
         */
        void UseLayout(const std::string& id);

        /**
         * Switch to fullscreen or windowed mode. If the mode
         * didn't change, the function does nothing.
         *
         * @param active if fullscreen should be active
         */
        void SetFullscreen(bool active);

        /**
         * Toggle between fullscreen and windowed mode.
         */
        void ToggleFullscreen();

        /**
         * Get the application resources manager.
         *
         * @return the application resources manager
         */
        ResourceManager& Resources();

        /**
         * Get the application event manager.
         *
         * @return the application event manager
         */
        EventManager& Events();

    private:
        void Init();
        void Loop();
        void Destroy();

        GLFWwindow* m_PHandle = nullptr;
        WindowState m_SavedState{};
        bool m_Fullscreen = false;

        ResourceManager m_Resources;
        EventManager m_Events;

        const Layout* m_PLayout = nullptr;

        bool m_InFrame = false;
        std::vector<ScheduleTask> m_Tasks;
    };
}
