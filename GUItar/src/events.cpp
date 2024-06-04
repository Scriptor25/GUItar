#include <iostream>
#include <guitar/events.hpp>

bool guitar::EventManager::Invoke(const std::string& id, const EventPayload* pPayload)
{
    bool consumed = false;
    for (const auto& [ptr, listener] : m_Listeners[id])
        consumed = consumed || listener(pPayload);
    return consumed;
}

void guitar::EventManager::Register(const std::string& id, const void* pOwner, const EventListener& listener)
{
    auto& ref = m_Listeners[id][pOwner];
    if (ref)
        std::cerr
                << "[EventManager] Warning: there already was a registered listener for '"
                << id
                << "' by pointer "
                << pOwner
                << std::endl;
    ref = listener;
}

void guitar::EventManager::Release(const std::string& id, const void* pOwner)
{
    m_Listeners[id].erase(pOwner);
}

guitar::EventPayload::EventPayload(const void* pSource)
    : PSource(pSource)
{
}

guitar::EventPayload::~EventPayload() = default;

guitar::SizePayload::SizePayload(const void* pSource, const int width, const int height)
    : EventPayload(pSource), Width(width), Height(height)
{
}

guitar::KeyPayload::KeyPayload(const void* pSource, const int key, const int scancode, const int action, const int mods)
    : EventPayload(pSource), Key(key), Scancode(scancode), Action(action), Mods(mods)
{
}

guitar::ImagePayload::ImagePayload(const void* pSource, ImTextureID& textureID, int& width, int& height)
    : EventPayload(pSource), TextureID(textureID), Width(width), Height(height)
{
}

guitar::StringPayload::StringPayload(const void* pSource, std::string& result)
    : EventPayload(pSource), Result(result)
{
}
