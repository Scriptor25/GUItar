#pragma once

#include <imgui.h>
#include <string>
#include <tinyxml2.h>
#include <guitar/guitar.hpp>

namespace guitar
{
    void GetStringAttrib(const tinyxml2::XMLElement* xml, const std::string& name, std::string& ref);
    void GetIntAttrib(const tinyxml2::XMLElement* xml, const std::string& name, int& ref);
    void GetBoolAttrib(const tinyxml2::XMLElement* xml, const std::string& name, bool& ref);
    void GetImVec2Attrib(const tinyxml2::XMLElement* xml, const std::string& name, ImVec2& ref);

    void FromXML(const tinyxml2::XMLElement* xml, AppConfig& ref);
    void FromXML(const tinyxml2::XMLElement* xml, Layout& ref);
    void FromXML(const tinyxml2::XMLElement* xml, Menu& ref);

    void FromXML(const tinyxml2::XMLElement* xml, std::unique_ptr<Element>& ref);

    void FromXML(const tinyxml2::XMLElement* xml, ButtonElement& ref);
    void FromXML(const tinyxml2::XMLElement* xml, CheckboxElement& ref);
    void FromXML(const tinyxml2::XMLElement* xml, ComboElement& ref);
    void FromXML(const tinyxml2::XMLElement* xml, CustomElement& ref);
    void FromXML(const tinyxml2::XMLElement* xml, DemoElement& ref);
    void FromXML(const tinyxml2::XMLElement* xml, GroupElement& ref);
    void FromXML(const tinyxml2::XMLElement* xml, ImageElement& ref);
    void FromXML(const tinyxml2::XMLElement* xml, InputTextElement& ref);
    void FromXML(const tinyxml2::XMLElement* xml, MenuItemElement& ref);
    void FromXML(const tinyxml2::XMLElement* xml, MenuBarElement& ref);
    void FromXML(const tinyxml2::XMLElement* xml, SimpleElement& ref);
    void FromXML(const tinyxml2::XMLElement* xml, TextElement& ref);
    void FromXML(const tinyxml2::XMLElement* xml, WindowElement& ref);
}
