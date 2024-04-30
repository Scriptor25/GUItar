#include <iostream>
#include <guitar/application.hpp>
#include <guitar/layout.hpp>
#include <guitar/xml.hpp>
#include <imgui.h>

std::string guitar::GetStringAttrib(const tinyxml2::XMLElement *xml, const std::string &name, const std::string &alt)
{
    const char *value;
    if (xml->QueryStringAttribute(name.c_str(), &value) == tinyxml2::XML_SUCCESS)
        return value;
    return alt;
}

int guitar::GetIntAttrib(const tinyxml2::XMLElement *xml, const std::string &name, const int alt)
{
    int value = alt;
    xml->QueryIntAttribute(name.c_str(), &value);
    return value;
}

void guitar::FromXML(const tinyxml2::XMLElement *xml, AppConfig &ref)
{
    ref.Title = GetStringAttrib(xml, "title", "");
    ref.Version = GetStringAttrib(xml, "version", "");
    ref.Layout = GetStringAttrib(xml, "layout", "");
    ref.Width = GetIntAttrib(xml, "width", 0);
    ref.Height = GetIntAttrib(xml, "height", 0);
}

void guitar::FromXML(const tinyxml2::XMLElement *xml, Layout &ref)
{
    ref.ID = GetStringAttrib(xml, "id", "");

    if (xml->NoChildren())
        return;

    for (auto ptr = xml->FirstChildElement(); ptr; ptr = ptr->NextSiblingElement())
    {
        Element *element = nullptr;
        FromXML(ref.Resources, ptr, element);
        if (element)
            ref.Elements.push_back(element);
    }
}

void guitar::FromXML(ResourceManager *resources, const tinyxml2::XMLElement *xml, Element *&ref)
{
    const std::string type = xml->Name();
    if (type == "dockspace")
    {
        const auto element = new DockSpaceElement(resources);
        FromXML(xml, *element);
        ref = element;
        return;
    }
    if (type == "demo")
    {
        const auto element = new DemoElement(resources);
        FromXML(xml, *element);
        ref = element;
        return;
    }
    if (type == "window")
    {
        const auto element = new WindowElement(resources);
        FromXML(xml, *element);
        ref = element;
        return;
    }
    if (type == "text")
    {
        const auto element = new TextElement(resources);
        FromXML(xml, *element);
        ref = element;
        return;
    }
    if (type == "button")
    {
        const auto element = new ButtonElement(resources);
        FromXML(xml, *element);
        ref = element;
        return;
    }

    std::cerr << "[Element FromXML] Undefined element type '" << type << "'" << std::endl;
}

void guitar::FromXML(const tinyxml2::XMLElement *xml, DockSpaceElement &ref)
{
}

void guitar::FromXML(const tinyxml2::XMLElement *xml, DemoElement &ref)
{
}

void guitar::FromXML(const tinyxml2::XMLElement *xml, WindowElement &ref)
{
    ref.Name = GetStringAttrib(xml, "name", "");

    if (xml->NoChildren())
        return;

    for (auto ptr = xml->FirstChildElement(); ptr; ptr = ptr->NextSiblingElement())
    {
        Element *element = nullptr;
        FromXML(ref.Resources, ptr, element);
        if (element)
            ref.Elements.push_back(element);
    }
}

void guitar::FromXML(const tinyxml2::XMLElement *xml, TextElement &ref)
{
    ref.Text = xml->GetText();
}

void guitar::FromXML(const tinyxml2::XMLElement *xml, ButtonElement &ref)
{
    ref.Text = xml->GetText();
    ref.Action = GetStringAttrib(xml, "action", "");
}

guitar::Layout::Layout()
        : Resources(nullptr)
{
}

guitar::Layout::Layout(guitar::ResourceManager *resources)
        : Resources(resources)
{
}

void guitar::Layout::Draw()
{
    for (const auto element: Elements)
        element->Draw();
}

guitar::Element::Element(guitar::ResourceManager *resources)
        : Resources(resources)
{
}

guitar::DockSpaceElement::DockSpaceElement(guitar::ResourceManager *resources)
        : Element(resources)
{
}

void guitar::DockSpaceElement::Draw()
{
    ImGui::DockSpaceOverViewport();
}

guitar::DemoElement::DemoElement(guitar::ResourceManager *resources)
        : Element(resources)
{
}

void guitar::DemoElement::Draw()
{
    ImGui::ShowDemoWindow();
}

guitar::WindowElement::WindowElement(guitar::ResourceManager *resources)
        : Element(resources)
{
}

void guitar::WindowElement::Draw()
{
    if (ImGui::Begin(Name.c_str()))
        for (const auto element: Elements)
            element->Draw();
    ImGui::End();
}

guitar::TextElement::TextElement(guitar::ResourceManager *resources)
        : Element(resources)
{
}

void guitar::TextElement::Draw()
{
    ImGui::TextUnformatted(Text.c_str());
}

guitar::ButtonElement::ButtonElement(guitar::ResourceManager *resources)
        : Element(resources)
{
}

void guitar::ButtonElement::Draw()
{
    if (ImGui::Button(Text.c_str()))
        if (!Action.empty())
            Resources->RunAction(Action);
}
