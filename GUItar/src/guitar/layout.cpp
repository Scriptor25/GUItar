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
        FromXML(ptr, element);
        if (element)
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
    if (type == "button")
    {
        const auto element = new ButtonElement();
        FromXML(xml, *element);
        ref = element;
        return;
    }

    std::cerr << "[FromXML] Undefined element type '" << type << "'" << std::endl;
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
        FromXML(ptr, element);
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

void guitar::Layout::Draw(EventManager &events)
{
    for (const auto element: Elements)
        element->Draw(events);
}

void guitar::DockSpaceElement::Draw(EventManager &events)
{
    ImGui::DockSpaceOverViewport();
}

void guitar::DemoElement::Draw(EventManager &events)
{
    ImGui::ShowDemoWindow();
}

void guitar::WindowElement::Draw(EventManager &events)
{
    if (ImGui::Begin(Name.c_str()))
        for (const auto element: Elements)
            element->Draw(events);
    ImGui::End();
}

void guitar::TextElement::Draw(EventManager &events)
{
    ImGui::TextUnformatted(Text.c_str());
}

void guitar::ButtonElement::Draw(EventManager &events)
{
    if (ImGui::Button(Text.c_str()))
        if (!Action.empty())
            events.Invoke(Action, new EventPayload(this));
}
