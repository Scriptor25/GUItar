#include <imgui.h>
#include <guitar/application.hpp>
#include <guitar/image.hpp>
#include <guitar/layout.hpp>

void guitar::Layout::Draw(ResourceManager& resources, EventManager& events) const
{
    for (const auto element : Elements)
        element->Draw(resources, events);
}

void guitar::DockSpaceElement::Draw(ResourceManager& resources, EventManager& events)
{
    ImGui::DockSpaceOverViewport();
}

void guitar::DemoElement::Draw(ResourceManager& resources, EventManager& events)
{
    ImGui::ShowDemoWindow();
}

void guitar::WindowElement::Draw(ResourceManager& resources, EventManager& events)
{
    if (ImGui::Begin(Name.c_str()))
        for (const auto element : Elements)
            element->Draw(resources, events);
    ImGui::End();
}

void guitar::ButtonElement::Draw(ResourceManager& resources, EventManager& events)
{
    if (ImGui::Button(Text.c_str()))
        if (!Action.empty())
            events.Invoke(Action, new EventPayload(this));
}

void guitar::ImageElement::Draw(ResourceManager& resources, EventManager& events)
{
    ImTextureID texture_id = nullptr;
    int width = 0, height = 0;
    if (Type == "resource")
    {
        const auto image = resources.GetImage(Source);
        width = image->Width;
        height = image->Height;
        texture_id = reinterpret_cast<ImTextureID>(static_cast<intptr_t>(image->Texture));
    }
    else if (Type == "event")
        events.Invoke(Source, new ImagePayload(this, &texture_id, &width, &height));

    ImVec2 bounds = Bounds;
    if (bounds.x <= 0 || bounds.y <= 0)
    {
        auto avail = ImGui::GetContentRegionAvail();
        if (avail.y <= 100) avail = ImGui::GetWindowSize();
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
    Func(Text.c_str());
}

void guitar::SimpleElement::Draw(ResourceManager& resources, EventManager& events)
{
    Func();
}
