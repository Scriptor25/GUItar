#include <iostream>
#include <guitar/events.hpp>

guitar::EventBase::EventBase(const void* pSource)
    : PSource(pSource)
{
}

guitar::EventBase::~EventBase() = default;

bool guitar::EventManager::Invoke(const std::string& id, const EventBase* pEvent)
{
    bool consumed = false;
    for (const auto& [ptr, listener] : m_Listeners[id])
        consumed = consumed || listener(pEvent);
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
