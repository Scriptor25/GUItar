#pragma once

#include <functional>
#include <map>
#include <string>

namespace guitar
{
    struct EventPayload
    {
        explicit EventPayload(void *source);

        virtual ~EventPayload();

        void *Source;
    };

    struct SizePayload : EventPayload
    {
        SizePayload(void *source, int width, int height);

        int Width;
        int Height;
    };

    struct KeyPayload : EventPayload
    {
        KeyPayload(void *source, int key, int scancode, int action, int mods);

        int Key;
        int Scancode;
        int Action;
        int Mods;
    };

    typedef std::function<bool(const EventPayload *payload)> EventListener;

    class EventManager
    {
    public:
        bool Invoke(const std::string &action, const EventPayload *payload);

        void Register(const std::string &action, const EventListener &listener);

        void Release(const std::string &action);

    private:
        std::map<std::string, EventListener> m_Listeners;
    };
}
