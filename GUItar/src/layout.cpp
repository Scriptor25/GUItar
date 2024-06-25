#define GLFW_INCLUDE_NONE

#include <algorithm>
#include <imgui.h>
#include <iomanip>
#include <iostream>
#include <map>
#include <ranges>
#include <GLFW/glfw3.h>
#include <guitar/events.hpp>
#include <guitar/layout.hpp>
#include <guitar/resources.hpp>

guitar::Element::~Element() = default;

void guitar::Element::Register(ResourceManager& resources, EventManager& events)
{
}

void guitar::Element::Release(ResourceManager& resources, EventManager& events)
{
}

guitar::Layout::Layout() = default;

guitar::Layout& guitar::Layout::operator=(Layout&& other) noexcept
{
    ID = other.ID;
    Dockspace = other.Dockspace;
    Elements.clear();
    for (auto& element : other.Elements)
        Elements.push_back(std::move(element));
    return *this;
}

void guitar::Layout::Register(ResourceManager& resources, EventManager& events) const
{
    for (const auto& element : Elements)
        element->Register(resources, events);
}

void guitar::Layout::Release(ResourceManager& resources, EventManager& events) const
{
    for (const auto& element : Elements)
        element->Release(resources, events);
}

void guitar::WindowElement::Register(ResourceManager& resources, EventManager& events)
{
    for (const auto& element : Elements)
        element->Register(resources, events);
}

void guitar::WindowElement::Release(ResourceManager& resources, EventManager& events)
{
    for (const auto& element : Elements)
        element->Release(resources, events);
}

void guitar::ComboElement::Register(ResourceManager& resources, EventManager& events)
{
    for (const auto& element : Elements)
        element->Register(resources, events);
}

void guitar::ComboElement::Release(ResourceManager& resources, EventManager& events)
{
    for (const auto& element : Elements)
        element->Release(resources, events);
}

void guitar::MenuItemElement::Register(ResourceManager& resources, EventManager& events)
{
    events.Register("on_key", this, [this, &events](const EventPayload* pPayload)
    {
        if (const auto& payload = *dynamic_cast<const KeyPayload*>(pPayload); Shortcut.Matches(payload))
        {
            const EventPayload evt_payload(this);
            events.Invoke(Event, &evt_payload);
            return true;
        }
        return false;
    });
}

void guitar::MenuItemElement::Release(ResourceManager& resources, EventManager& events)
{
    events.Release("on_key", this);
}

void guitar::Menu::Register(ResourceManager& resources, EventManager& events) const
{
    for (const auto& element : Elements)
        element->Register(resources, events);
}

void guitar::Menu::Release(ResourceManager& resources, EventManager& events) const
{
    for (const auto& element : Elements)
        element->Release(resources, events);
}

void guitar::MenuBarElement::Register(ResourceManager& resources, EventManager& events)
{
    for (const auto& menu : Menus)
        menu.Register(resources, events);
}

void guitar::MenuBarElement::Release(ResourceManager& resources, EventManager& events)
{
    for (const auto& menu : Menus)
        menu.Release(resources, events);
}

void guitar::CheckboxElement::Register(ResourceManager& resources, EventManager& events)
{
    Var = resources.GetString(events, Default) == "true";
}

std::map<std::string, guitar::TextFunc> guitar::TextElement::FUNCS = {
    {"text", ImGui::Text},
    {"separatorText", ImGui::SeparatorText},
};

std::map<std::string, std::function<void()>> guitar::SimpleElement::FUNCS = {
    {"bullet", ImGui::Bullet},
    {"separator", ImGui::Separator},
    {"spacing", ImGui::Spacing},
    {"newline", ImGui::NewLine}
};

guitar::KeyShortcut::KeyShortcut() = default;

static std::map<std::string, int> keys
{
    {"ESCAPE", GLFW_KEY_ESCAPE},
    {"ENTER", GLFW_KEY_ENTER},
    {"TAB", GLFW_KEY_TAB},
    {"BACKSPACE", GLFW_KEY_BACKSPACE},
    {"INSERT", GLFW_KEY_INSERT},
    {"DELETE", GLFW_KEY_DELETE},
    {"RIGHT", GLFW_KEY_RIGHT},
    {"LEFT", GLFW_KEY_LEFT},
    {"DOWN", GLFW_KEY_DOWN},
    {"UP", GLFW_KEY_UP},
    {"PAGE_UP", GLFW_KEY_PAGE_UP},
    {"PAGE_DOWN", GLFW_KEY_PAGE_DOWN},
    {"HOME", GLFW_KEY_HOME},
    {"END", GLFW_KEY_END},
    {"CAPS_LOCK", GLFW_KEY_CAPS_LOCK},
    {"SCROLL_LOCK", GLFW_KEY_SCROLL_LOCK},
    {"NUM_LOCK", GLFW_KEY_NUM_LOCK},
    {"PRINT_SCREEN", GLFW_KEY_PRINT_SCREEN},
    {"PAUSE", GLFW_KEY_PAUSE},
    {"F1", GLFW_KEY_F1},
    {"F2", GLFW_KEY_F2},
    {"F3", GLFW_KEY_F3},
    {"F4", GLFW_KEY_F4},
    {"F5", GLFW_KEY_F5},
    {"F6", GLFW_KEY_F6},
    {"F7", GLFW_KEY_F7},
    {"F8", GLFW_KEY_F8},
    {"F9", GLFW_KEY_F9},
    {"F10", GLFW_KEY_F10},
    {"F11", GLFW_KEY_F11},
    {"F12", GLFW_KEY_F12},
    {"F13", GLFW_KEY_F13},
    {"F14", GLFW_KEY_F14},
    {"F15", GLFW_KEY_F15},
    {"F16", GLFW_KEY_F16},
    {"F17", GLFW_KEY_F17},
    {"F18", GLFW_KEY_F18},
    {"F19", GLFW_KEY_F19},
    {"F20", GLFW_KEY_F20},
    {"F21", GLFW_KEY_F21},
    {"F22", GLFW_KEY_F22},
    {"F23", GLFW_KEY_F23},
    {"F24", GLFW_KEY_F24},
    {"F25", GLFW_KEY_F25},
    {"KP_0", GLFW_KEY_KP_0},
    {"KP_1", GLFW_KEY_KP_1},
    {"KP_2", GLFW_KEY_KP_2},
    {"KP_3", GLFW_KEY_KP_3},
    {"KP_4", GLFW_KEY_KP_4},
    {"KP_5", GLFW_KEY_KP_5},
    {"KP_6", GLFW_KEY_KP_6},
    {"KP_7", GLFW_KEY_KP_7},
    {"KP_8", GLFW_KEY_KP_8},
    {"KP_9", GLFW_KEY_KP_9},
    {"KP_DECIMAL", GLFW_KEY_KP_DECIMAL},
    {"KP_DIVIDE", GLFW_KEY_KP_DIVIDE},
    {"KP_MULTIPLY", GLFW_KEY_KP_MULTIPLY},
    {"KP_SUBTRACT", GLFW_KEY_KP_SUBTRACT},
    {"KP_ADD", GLFW_KEY_KP_ADD},
    {"KP_ENTER", GLFW_KEY_KP_ENTER},
    {"KP_EQUAL", GLFW_KEY_KP_EQUAL},
    {"LEFT_SHIFT", GLFW_KEY_LEFT_SHIFT},
    {"LEFT_CONTROL", GLFW_KEY_LEFT_CONTROL},
    {"LEFT_ALT", GLFW_KEY_LEFT_ALT},
    {"LEFT_SUPER", GLFW_KEY_LEFT_SUPER},
    {"RIGHT_SHIFT", GLFW_KEY_RIGHT_SHIFT},
    {"RIGHT_CONTROL", GLFW_KEY_RIGHT_CONTROL},
    {"RIGHT_ALT", GLFW_KEY_RIGHT_ALT},
    {"RIGHT_SUPER", GLFW_KEY_RIGHT_SUPER},
    {"MENU", GLFW_KEY_MENU},
};

std::vector<std::string> split(const std::string& str, const char delim)
{
    std::vector<std::string> substrs;
    auto strcpy = str;

    for (auto pos = strcpy.find(delim); pos != std::string::npos; pos = strcpy.find(delim))
    {
        if (auto substr = strcpy.substr(0, pos); !substr.empty())
            substrs.push_back(substr);
        strcpy = strcpy.substr(pos + 1);
    }

    if (!strcpy.empty())
        substrs.push_back(strcpy);

    return substrs;
}

guitar::KeyShortcut::KeyShortcut(const std::string& str)
{
    for (const auto& substr : split(str, '|'))
    {
        auto& [Key, Ctrl, Alt, Super, Shift] = Infos.emplace_back();
        for (auto info_str : split(substr, '+'))
        {
            std::ranges::transform(info_str, info_str.begin(), [](const unsigned char c) { return std::toupper(c); });

            if (info_str == "CTRL") Ctrl = true;
            else if (info_str == "ALT") Alt = true;
            else if (info_str == "SUPER") Super = true;
            else if (info_str == "SHIFT") Shift = true;
            else
            {
                if (const auto& key = keys[info_str])
                    Key = key;
                else if (info_str.size() == 1)
                    Key = static_cast<unsigned char>(info_str[0]);
                else std::cerr << "Undefined key identifier " << std::quoted(info_str) << std::endl;
            }
        }
    }
}

bool guitar::KeyShortcut::Matches(const KeyPayload& payload) const
{
    if (payload.Action != GLFW_RELEASE)
        return false;

    return std::ranges::any_of(Infos, [payload](const ShortcutInfo& info)
    {
        return info.Key == payload.Key
                && info.Ctrl == ((payload.Mods & GLFW_MOD_CONTROL) != 0)
                && info.Alt == ((payload.Mods & GLFW_MOD_ALT) != 0)
                && info.Super == ((payload.Mods & GLFW_MOD_SUPER) != 0)
                && info.Shift == ((payload.Mods & GLFW_MOD_SHIFT) != 0);
    });
}

std::string guitar::KeyShortcut::String() const
{
    std::string str;
    for (size_t i = 0; i < Infos.size(); ++i)
    {
        const auto& [KeyCode, Ctrl, Alt, Super, Shift] = Infos[i];
        if (i > 0) str += ", ";
        if (Ctrl) str += "CTRL+";
        if (Alt) str += "ALT+";
        if (Super) str += "SUPER+";
        if (Shift) str += "SHIFT+";

        bool found_keycode = false;
        for (const auto& [id, keycode] : keys)
        {
            if (keycode == KeyCode)
            {
                str += id;
                found_keycode = true;
                break;
            }
        }
        if (!found_keycode)
            str += static_cast<char>(KeyCode);
    }
    return str;
}
