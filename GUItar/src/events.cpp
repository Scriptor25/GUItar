#include <iostream>
#include <guitar/events.hpp>

bool guitar::EventManager::Invoke(const std::string& action, EventPayload* payload)
{
    bool consumed = false;
    for (const auto& [ptr, listener] : m_Listeners[action])
        consumed = consumed || listener(payload);
    return consumed;
}

void guitar::EventManager::Register(const std::string& action, const void* ptr, const EventListener& listener)
{
    auto& ref = m_Listeners[action][ptr];
    if (ref)
        std::cerr
                << "[EventManager] Warning: there already was a registered listener for '"
                << action
                << "' by pointer "
                << ptr
                << std::endl;
    ref = listener;
}

void guitar::EventManager::Release(const std::string& action, const void* ptr)
{
    m_Listeners[action].erase(ptr);
}

guitar::EventPayload::EventPayload(void* source)
    : Source(source)
{
}

guitar::EventPayload::~EventPayload() = default;

guitar::SizePayload::SizePayload(void* source, const int width, const int height)
    : EventPayload(source), Width(width), Height(height)
{
}

guitar::KeyPayload::KeyPayload(void* source, const int key, const int scancode, const int action, const int mods)
    : EventPayload(source), Key(key), Scancode(scancode), Action(action), Mods(mods)
{
}

guitar::ImagePayload::ImagePayload(void* source, ImTextureID* pTextureID, int* pWidth, int* pHeight)
    : EventPayload(source), PTextureID(pTextureID), PWidth(pWidth), PHeight(pHeight)
{
}
