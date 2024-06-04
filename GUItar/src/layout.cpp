#include <imgui.h>
#include <map>
#include <guitar/layout.hpp>

guitar::Element::~Element() = default;

guitar::Layout::Layout() = default;

guitar::Layout& guitar::Layout::operator=(Layout&& other) noexcept
{
    ID = other.ID;
    Elements.clear();
    for (auto& element : other.Elements)
        Elements.push_back(std::move(element));
    return *this;
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
