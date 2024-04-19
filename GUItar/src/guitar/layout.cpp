#include <iostream>
#include <guitar/application.hpp>
#include <guitar/layout.hpp>
#include <guitar/xml.hpp>
#include <imgui/imgui.h>

void guitar::FromXML(const tinyxml2::XMLElement *xml, guitar::AppConfig &ref)
{
    const char *title;
    const char *version;
    const char *layout;
    xml->QueryStringAttribute("title", &title);
    xml->QueryStringAttribute("version", &version);
    xml->QueryStringAttribute("layout", &layout);

    ref.Title = title;
    ref.Version = version;
    ref.Layout = layout;
    xml->QueryIntAttribute("width", &ref.Width);
    xml->QueryIntAttribute("height", &ref.Height);
}

void guitar::FromXML(const tinyxml2::XMLElement *xml, Layout &ref)
{
    const char *id;
    xml->QueryStringAttribute("id", &id);
    ref.ID = id;

    if (xml->NoChildren())
        return;

    for (auto ptr = xml->FirstChildElement(); ptr; ptr = ptr->NextSiblingElement())
    {
        Element *element;
        FromXML(ptr, element);
        ref.Elements.push_back(element);
    }
}

void guitar::FromXML(const tinyxml2::XMLElement *xml, Element *&ref)
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
    if (type == "window")
    {
        const auto element = new WindowElement();
        FromXML(xml, *element);
        ref = element;
        return;
    }
    if (type == "text")
    {
        const auto element = new TextElement();
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

void guitar::FromXML(const tinyxml2::XMLElement *xml, guitar::WindowElement &ref)
{
    const char *name;
    xml->QueryStringAttribute("name", &name);
    ref.Name = name;

    if (xml->NoChildren())
        return;

    for (auto ptr = xml->FirstChildElement(); ptr; ptr = ptr->NextSiblingElement())
    {
        Element *element;
        FromXML(ptr, element);
        ref.Elements.push_back(element);
    }
}

void guitar::FromXML(const tinyxml2::XMLElement *xml, guitar::TextElement &ref)
{
    ref.Text = xml->GetText();
}

void guitar::Layout::Draw()
{
    for (const auto element: Elements)
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

void guitar::WindowElement::Draw()
{
    if (ImGui::Begin(Name.c_str()))
        for (auto element: Elements)
            element->Draw();
    ImGui::End();
}

void guitar::TextElement::Draw()
{
    ImGui::TextUnformatted(Text.c_str());
}
