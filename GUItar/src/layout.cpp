#include <imgui.h>
#include <map>
#include <guitar/layout.hpp>

std::map<std::string, std::function<void(const char*)>> guitar::TextElement::FUNCS = {
    {"text", ImGui::Text},
    {"separatorText", ImGui::SeparatorText},
};

std::map<std::string, std::function<void()>> guitar::SimpleElement::FUNCS = {
    {"bullet", ImGui::Bullet},
    {"separator", ImGui::Separator},
    {"spacing", ImGui::Spacing},
    {"newline", ImGui::NewLine}
};
