#include <iostream>
#include <guitar/layout.hpp>
#include <guitar/xml.hpp>
#include <imgui/imgui.h>

void guitar::FromXML(const tinyxml2::XMLElement* xml, Layout& ref)
{
    const char* id;
    xml->QueryStringAttribute("id", &id);
    ref.ID = id;

    if (xml->NoChildren())
        return;

    for (auto ptr = xml->FirstChildElement(); ptr; ptr = ptr->NextSiblingElement())
    {
        Element* element;
        FromXML(ptr, element);
        ref.Elements.push_back(element);
    }
}

void guitar::FromXML(const tinyxml2::XMLElement* xml, Element*& ref)
{
    const std::string type = xml->Name();
    if (type == "dockspace")
    {
        const auto element = new DockSpaceElement();
        FromXML(xml, *element);
        ref = element;
        return;
    }
    if (type == "demo")
    {
        const auto element = new DemoElement();
        FromXML(xml, *element);
        ref = element;
        return;
    }

    std::cerr << "[Element FromXML] Undefined element type '" << type << "'" << std::endl;
}

void guitar::FromXML(const tinyxml2::XMLElement* xml, DockSpaceElement& ref)
{
}

void guitar::FromXML(const tinyxml2::XMLElement* xml, DemoElement& ref)
{
}

void guitar::Layout::Draw()
{
    for (const auto element : Elements)
        element->Draw();
}

void guitar::DockSpaceElement::Draw()
{
    ImGui::DockSpaceOverViewport();
}

void guitar::DemoElement::Draw()
{
    ImGui::ShowDemoWindow();
}
