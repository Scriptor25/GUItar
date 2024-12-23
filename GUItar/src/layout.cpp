#define GLFW_INCLUDE_NONE

#include <algorithm>
#include <imgui.h>
#include <iomanip>
#include <iostream>
#include <map>
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

void guitar::GroupElement::Register(ResourceManager& resources, EventManager& events)
{
    for (const auto& element : Elements)
        element->Register(resources, events);
}

void guitar::GroupElement::Release(ResourceManager& resources, EventManager& events)
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
    events.Register("on_key", this, [this, &events](const EventBase* payload_ptr)
    {
        if (const auto& payload = *dynamic_cast<const ImmutableEvent<KeyPayload>*>(payload_ptr);
            Shortcut.Matches(payload))
        {
            const EventBase event(this);
            events.Invoke(Event, &event);
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
    {"newline", ImGui::NewLine},
    {"sameline", [] { ImGui::SameLine(); }},
};

guitar::KeyShortcut::KeyShortcut() = default;

static const std::map<std::string, int> KEYS
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
    std::vector<std::string> sub_strs;
    auto str_cpy = str;

    for (auto pos = str_cpy.find(delim); pos != std::string::npos; pos = str_cpy.find(delim))
    {
        if (auto substr = str_cpy.substr(0, pos); !substr.empty())
            sub_strs.push_back(substr);
        str_cpy = str_cpy.substr(pos + 1);
    }

    if (!str_cpy.empty())
        sub_strs.push_back(str_cpy);

    return sub_strs;
}

guitar::KeyShortcut::KeyShortcut(const std::string& str)
{
    for (const auto& substr : split(str, '|'))
    {
        auto& [key_, ctrl_, alt_, super_, shift_] = Infos.emplace_back();
        for (auto info_str : split(substr, '+'))
        {
            for (auto& c : info_str)
                c = static_cast<char>(std::toupper(c));

            if (info_str == "CTRL") ctrl_ = true;
            else if (info_str == "ALT") alt_ = true;
            else if (info_str == "SUPER") super_ = true;
            else if (info_str == "SHIFT") shift_ = true;
            else
            {
                if (KEYS.contains(info_str))
                    key_ = KEYS.at(info_str);
                else if (info_str.size() == 1)
                    key_ = static_cast<unsigned char>(info_str[0]);
                else std::cerr << "Undefined key identifier " << std::quoted(info_str) << std::endl;
            }
        }
    }
}

bool guitar::KeyShortcut::Matches(const ImmutableEvent<KeyPayload>& event) const
{
    if (event.Payload.Action != GLFW_RELEASE)
        return false;

    return std::ranges::any_of(Infos, [&event](const ShortcutInfo& info)
    {
        return info.Key == event.Payload.Key
            && info.Ctrl == ((event.Payload.Mods & GLFW_MOD_CONTROL) != 0)
            && info.Alt == ((event.Payload.Mods & GLFW_MOD_ALT) != 0)
            && info.Super == ((event.Payload.Mods & GLFW_MOD_SUPER) != 0)
            && info.Shift == ((event.Payload.Mods & GLFW_MOD_SHIFT) != 0);
    });
}

std::string guitar::KeyShortcut::String() const
{
    std::string str;
    for (size_t i = 0; i < Infos.size(); ++i)
    {
        const auto& [key_code_, ctrl_, alt_, super_, shift_] = Infos[i];
        if (i > 0) str += ", ";
        if (ctrl_) str += "CTRL+";
        if (alt_) str += "ALT+";
        if (super_) str += "SUPER+";
        if (shift_) str += "SHIFT+";

        bool found_keycode = false;
        for (const auto& [id_, code_] : KEYS)
        {
            if (code_ == key_code_)
            {
                str += id_;
                found_keycode = true;
                break;
            }
        }
        if (!found_keycode)
            str += static_cast<char>(key_code_);
    }
    return str;
}
