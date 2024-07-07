#pragma once

#include <functional>
#include <imgui.h>
#include <map>
#include <string>

namespace guitar
{
    struct SizePayload
    {
        int Width;
        int Height;
    };

    struct KeyPayload
    {
        int KeyCode;
        int ScanCode;
        int Action;
        int Mods;
    };

    struct ImagePayload
    {
        ImTextureID& TextureID;
        int& Width;
        int& Height;
    };

    struct EventBase
    {
        explicit EventBase(const void* pSource);
        virtual ~EventBase();

        const void* PSource;
    };

    template <typename T>
    struct ImmutableEvent : EventBase
    {
        ImmutableEvent(const void* pSource, const T& payload)
            : EventBase(pSource), Payload(payload)
        {
        }

        const T& Payload;
    };

    template <typename T>
    struct MutableEvent : EventBase
    {
        MutableEvent(const void* pSource, T& payload)
            : EventBase(pSource), Payload(payload)
        {
        }

        T& Payload;
    };

    using SizeEvent = ImmutableEvent<SizePayload>;
    using KeyEvent = ImmutableEvent<KeyPayload>;
    using ImageEvent = ImmutableEvent<ImagePayload>;

    typedef std::function<bool(const EventBase*)> EventListener;

    /**
     * The EventManager serves as a simple event system.
     * You can invoke an event, register a listerner for one
     * and release the listerner.
     */
    class EventManager
    {
    public:
        /**
         * Manually invoke an event.
         *
         * @param id the action id
         * @param pEvent a payload containing event information
         * @return if the event got captured by a listener
         */
        bool Invoke(const std::string& id, const EventBase* pEvent);

        /**
         * Register a listener for an action, owned by a pointer.
         *
         * @param id the action id
         * @param pOwner the owners pointer
         * @param listener the listener function
         */
        void Register(const std::string& id, const void* pOwner, const EventListener& listener);

        /**
         * Release a listener.
         *
         * @param id the action id
         * @param pOwner the owners pointer
         */
        void Release(const std::string& id, const void* pOwner);

    private:
        std::map<std::string, std::map<const void*, EventListener>> m_Listeners;
    };
}
