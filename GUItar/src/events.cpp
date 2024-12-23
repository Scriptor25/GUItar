#include <iostream>
#include <guitar/events.hpp>

guitar::EventBase::EventBase(const void* source)
    : UserPtr(source)
{
}

guitar::EventBase::~EventBase() = default;

bool guitar::EventManager::Invoke(const std::string& id, const EventBase* event_ptr)
{
    bool consumed = false;
    for (const auto& [user_ptr_, listener_] : m_Listeners[id])
        consumed = consumed || listener_(event_ptr);
    return consumed;
}

void guitar::EventManager::Register(const std::string& id, const void* user_ptr, const EventListener& listener)
{
    auto& ref = m_Listeners[id][user_ptr];
    if (ref)
        std::cerr
            << "[EventManager] Warning: there already was a registered listener for '"
            << id
            << "' for pointer "
            << user_ptr
            << std::endl;
    ref = listener;
}

void guitar::EventManager::Release(const std::string& id, const void* user_ptr)
{
    m_Listeners[id].erase(user_ptr);
}
