#include <imgui.h>
#include <guitar/application.hpp>
#include <guitar/image.hpp>
#include <guitar/layout.hpp>

void guitar::Layout::Draw(ResourceManager& resources, EventManager& events) const
{
    for (const auto& element : Elements)
        element->Draw(resources, events);
}

void guitar::DockSpaceElement::Draw(ResourceManager&, EventManager&)
{
    ImGui::DockSpaceOverViewport();
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

void guitar::ButtonElement::Draw(ResourceManager& resources, EventManager& events)
{
    const auto text = resources.GetString(events, Text);
    if (ImGui::Button(text.c_str()))
        if (!Event.empty())
        {
            const EventPayload payload(this);
            events.Invoke(Event, &payload);
        }
}

void guitar::ImageElement::Draw(ResourceManager& resources, EventManager& events)
{
    ImTextureID texture_id = nullptr;
    int width = 0, height = 0;

    if (Source[0] == '$')
    {
        const ImagePayload payload(this, texture_id, width, height);
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
    const EventPayload payload(this);
    events.Invoke(Event, &payload);
}
