#include <imgui.h>
#include <guitar/application.hpp>
#include <guitar/image.hpp>
#include <guitar/layout.hpp>
#include <misc/cpp/imgui_stdlib.h>

void guitar::Layout::Draw(ResourceManager& resources, EventManager& events) const
{
    if (Dockspace) ImGui::DockSpaceOverViewport();
    for (const auto& element : Elements)
        element->Draw(resources, events);
}

void guitar::DemoElement::Draw(ResourceManager&, EventManager&)
{
    ImGui::ShowDemoWindow();
}

void guitar::WindowElement::Draw(ResourceManager& resources, EventManager& events)
{
    const auto name = resources.GetString(events, Name);
    if (ImGui::Begin(name.c_str()))
        for (const auto& element : Elements)
            element->Draw(resources, events);
    ImGui::End();
}

void guitar::GroupElement::Draw(ResourceManager& resources, EventManager& events)
{
    ImGui::BeginGroup();
    for (const auto& element : Elements)
        element->Draw(resources, events);
    ImGui::EndGroup();
}

void guitar::ButtonElement::Draw(ResourceManager& resources, EventManager& events)
{
    const auto text = resources.GetString(events, Text);
    if (ImGui::Button(text.c_str()))
        if (!Event.empty())
        {
            const EventBase event(this);
            events.Invoke(Event, &event);
        }
}

void guitar::ImageElement::Draw(ResourceManager& resources, EventManager& events)
{
    ImTextureID texture_id = nullptr;
    int width = 0, height = 0;

    if (Source[0] == '$')
    {
        const ImmutableEvent payload(this, ImagePayload{texture_id, width, height});
        events.Invoke(Source.substr(1), &payload);
    }
    else
    {
        if (const auto image = resources.GetImage(Source))
        {
            width = image->Width;
            height = image->Height;
            texture_id = reinterpret_cast<ImTextureID>(static_cast<intptr_t>(image->Texture));
        }
    }

    if (!texture_id || width <= 0 || height <= 0)
        return;

    ImVec2 bounds = Bounds;
    if (bounds.x <= 0 || bounds.y <= 0)
    {
        auto avail = ImGui::GetContentRegionAvail();
        if (avail.y <= 0) avail = ImGui::GetWindowSize();
        const auto padding = ImGui::GetStyle().ItemSpacing;
        bounds.x = avail.x - padding.x;
        bounds.y = avail.y - padding.y;
    }

    ImVec2 size = bounds;
    if (KeepRatio)
    {
        const auto scale = std::min(bounds.x / static_cast<float>(width), bounds.y / static_cast<float>(height));
        size.x = static_cast<float>(width) * scale;
        size.y = static_cast<float>(height) * scale;
    }

    ImGui::Image(texture_id, size, UV0, UV1);
}

void guitar::TextElement::Draw(ResourceManager& resources, EventManager& events)
{
    const auto text = resources.GetString(events, Text);
    Func(text.c_str());
}

void guitar::SimpleElement::Draw(ResourceManager&, EventManager&)
{
    Func();
}

void guitar::ComboElement::Draw(ResourceManager& resources, EventManager& events)
{
    const auto label = resources.GetString(events, Label);
    const auto preview = resources.GetString(events, Preview);

    if (ImGui::BeginCombo(label.c_str(), preview.c_str()))
    {
        for (const auto& element : Elements)
            element->Draw(resources, events);
        ImGui::EndCombo();
    }
}

void guitar::CustomElement::Draw(ResourceManager&, EventManager& events)
{
    const EventBase payload(this);
    events.Invoke(Event, &payload);
}

void guitar::InputTextElement::Register(ResourceManager& resources, EventManager& events)
{
    Var = resources.GetString(events, Default);
}

void guitar::InputTextElement::Draw(ResourceManager& resources, EventManager& events)
{
    const auto label = resources.GetString(events, Label);
    const auto hint = resources.GetString(events, Hint);

    if (ImGui::InputTextWithHint(label.c_str(), hint.c_str(), &Var, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        const ImmutableEvent payload(this, Var);
        events.Invoke(Event, &payload);
    }
}

void guitar::MenuItemElement::Draw(ResourceManager& resources, EventManager& events)
{
    const auto label = resources.GetString(events, Label);
    const auto shortcut = Shortcut.String();

    if (ImGui::MenuItem(label.c_str(), shortcut.c_str()))
    {
        const EventBase payload(this);
        events.Invoke(Event, &payload);
    }
}

void guitar::Menu::Draw(ResourceManager& resources, EventManager& events) const
{
    const auto label = resources.GetString(events, Label);

    if (ImGui::BeginMenu(label.c_str()))
    {
        for (const auto& item : Elements)
            item->Draw(resources, events);
        ImGui::EndMenu();
    }
}

void guitar::MenuBarElement::Draw(ResourceManager& resources, EventManager& events)
{
    if (ImGui::BeginMainMenuBar())
    {
        for (const auto& menu : Menus)
            menu.Draw(resources, events);
        ImGui::EndMainMenuBar();
    }
}

void guitar::CheckboxElement::Draw(ResourceManager& resources, EventManager& events)
{
    const auto label = resources.GetString(events, Label);

    if (ImGui::Checkbox(label.c_str(), &Var))
    {
        const EventBase payload(this);
        events.Invoke(Event, &payload);
    }
}
