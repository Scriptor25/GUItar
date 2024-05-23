#pragma once

#include <functional>
#include <imgui.h>
#include <map>
#include <string>

namespace guitar
{
    /**
     * A payload containing information about an event.
     * For simple events it only contains the source, for
     * size or key callbacks you have to cast to a
     * @code SizePayload@endcode or @code KeyPayload@endcode
     * respectively.
     */
    struct EventPayload
    {
        explicit EventPayload(void* pSource);

        virtual ~EventPayload();

        void* PSource;
    };

    /**
     * Contains the new width and height along with the
     * source of the event.
     */
    struct SizePayload : EventPayload
    {
        SizePayload(void* pSource, int width, int height);

        int Width;
        int Height;
    };

    /**
     * Contains the key code, scancode, action and mod key flags
     * of a key event along with the source.
     */
    struct KeyPayload : EventPayload
    {
        KeyPayload(void* pSource, int key, int scancode, int action, int mods);

        int Key;
        int Scancode;
        int Action;
        int Mods;
    };

    struct ImagePayload : EventPayload
    {
        ImagePayload(void* pSource, ImTextureID* pTextureID, int* pWidth, int* pHeight);

        ImTextureID* PTextureID;
        int *PWidth, *PHeight;
    };

    typedef std::function<bool(EventPayload* pPayload)> EventListener;

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
         * @param pPayload a payload containing event information
         * @return if the event got captured by a listener
         */
        bool Invoke(const std::string& id, EventPayload* pPayload);

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
