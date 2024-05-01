#include <guitar/events.hpp>
#include <iostream>

bool guitar::EventManager::Invoke(const std::string &action, const EventPayload *payload)
{
    const auto &listener = m_Listeners[action];
    if (listener) return listener(payload);
    return false;
}

void guitar::EventManager::Register(const std::string &action, const guitar::EventListener &listener)
{
    auto &ref = m_Listeners[action];
    if (ref)
        std::cerr
                << "[EventManager] Warning: there already was a registered listener for '"
                << action
                << "'"
                << std::endl;
    ref = listener;
}

void guitar::EventManager::Release(const std::string &action)
{
    m_Listeners.erase(action);
}

guitar::EventPayload::EventPayload(void *source)
        : Source(source)
{
}

guitar::EventPayload::~EventPayload() = default;

guitar::SizePayload::SizePayload(void *source, const int width, const int height)
        : EventPayload(source), Width(width), Height(height)
{
}

guitar::KeyPayload::KeyPayload(void *source, const int key, const int scancode, const int action, const int mods)
        : EventPayload(source), Key(key), Scancode(scancode), Action(action), Mods(mods)
{
}
