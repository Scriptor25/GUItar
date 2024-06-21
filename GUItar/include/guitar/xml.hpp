#pragma once

#include <imgui.h>
#include <string>
#include <tinyxml2.h>
#include <guitar/guitar.hpp>

namespace guitar
{
    void GetStringAttrib(const tinyxml2::XMLElement* pXml, const std::string& name, std::string& ref);
    void GetIntAttrib(const tinyxml2::XMLElement* pXml, const std::string& name, int& ref);
    void GetBoolAttrib(const tinyxml2::XMLElement* pXml, const std::string& name, bool& ref);
    void GetImVec2Attrib(const tinyxml2::XMLElement* pXml, const std::string& name, ImVec2& ref);

    void FromXML(const tinyxml2::XMLElement* pXml, AppConfig& ref);
    void FromXML(const tinyxml2::XMLElement* pXml, Layout& ref);
    void FromXML(const tinyxml2::XMLElement* pXml, std::unique_ptr<Element>& ref);
    void FromXML(const tinyxml2::XMLElement* pXml, DockSpaceElement& ref);
    void FromXML(const tinyxml2::XMLElement* pXml, DemoElement& ref);
    void FromXML(const tinyxml2::XMLElement* pXml, WindowElement& ref);
    void FromXML(const tinyxml2::XMLElement* pXml, ButtonElement& ref);
    void FromXML(const tinyxml2::XMLElement* pXml, ImageElement& ref);
    void FromXML(const tinyxml2::XMLElement* pXml, ComboElement& ref);
    void FromXML(const tinyxml2::XMLElement* pXml, CustomElement& ref);
    void FromXML(const tinyxml2::XMLElement* pXml, InputTextElement& ref);
}
