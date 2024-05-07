#include <iostream>
#include <guitar/layout.hpp>
#include <guitar/xml.hpp>

void guitar::GetStringAttrib(const tinyxml2::XMLElement* xml, const std::string& name, std::string& ref)
{
    const char* value;
    if (xml->QueryStringAttribute(name.c_str(), &value) == tinyxml2::XML_SUCCESS)
        ref = value;
}

void guitar::GetIntAttrib(const tinyxml2::XMLElement* xml, const std::string& name, int& ref)
{
    xml->QueryIntAttribute(name.c_str(), &ref);
}

void guitar::GetBoolAttrib(const tinyxml2::XMLElement* xml, const std::string& name, bool& ref)
{
    xml->QueryBoolAttribute(name.c_str(), &ref);
}

void guitar::GetImVec2Attrib(const tinyxml2::XMLElement* xml, const std::string& name, ImVec2& ref)
{
    std::string value;
    GetStringAttrib(xml, name, value);
    if (!value.empty())
    {
        const auto idx = value.find_first_of(';');
        ref.x = std::stof(value.substr(0, idx));
        ref.y = std::stof(value.substr(idx + 1));
    }
}

void guitar::FromXML(const tinyxml2::XMLElement* xml, AppConfig& ref)
{
    GetStringAttrib(xml, "title", ref.Title);
    GetStringAttrib(xml, "version", ref.Version);
    GetStringAttrib(xml, "layout", ref.Layout);
    GetIntAttrib(xml, "width", ref.Width);
    GetIntAttrib(xml, "height", ref.Height);
}

void guitar::FromXML(const tinyxml2::XMLElement* xml, Layout& ref)
{
    GetStringAttrib(xml, "id", ref.ID);

    if (xml->NoChildren())
        return;

    for (auto ptr = xml->FirstChildElement(); ptr; ptr = ptr->NextSiblingElement())
    {
        Element* element = nullptr;
        FromXML(ptr, element);
        if (element)
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
    if (type == "window")
    {
        const auto element = new WindowElement();
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
    if (type == "image")
    {
        const auto element = new ImageElement();
        FromXML(xml, *element);
        ref = element;
        return;
    }

    if (const auto& func = TextElement::FUNCS[type])
    {
        const auto element = new TextElement();
        element->Func = func;
        element->Text = xml->GetText();
        ref = element;
        return;
    }

    if (const auto& func = SimpleElement::FUNCS[type])
    {
        const auto element = new SimpleElement();
        element->Func = func;
        ref = element;
        return;
    }

    std::cerr << "[FromXML] Undefined element type '" << type << "'" << std::endl;
}

void guitar::FromXML(const tinyxml2::XMLElement*, DockSpaceElement&)
{
}

void guitar::FromXML(const tinyxml2::XMLElement*, DemoElement&)
{
}

void guitar::FromXML(const tinyxml2::XMLElement* xml, WindowElement& ref)
{
    GetStringAttrib(xml, "name", ref.Name);

    if (xml->NoChildren())
        return;

    for (auto ptr = xml->FirstChildElement(); ptr; ptr = ptr->NextSiblingElement())
    {
        Element* element = nullptr;
        FromXML(ptr, element);
        if (element)
            ref.Elements.push_back(element);
    }
}

void guitar::FromXML(const tinyxml2::XMLElement* xml, ButtonElement& ref)
{
    ref.Text = xml->GetText();
    GetStringAttrib(xml, "action", ref.Action);
}

void guitar::FromXML(const tinyxml2::XMLElement* xml, ImageElement& ref)
{
    GetStringAttrib(xml, "src", ref.Source);
    GetStringAttrib(xml, "type", ref.Type);
    GetBoolAttrib(xml, "keepRatio", ref.KeepRatio);
    GetImVec2Attrib(xml, "bounds", ref.Bounds);
    GetImVec2Attrib(xml, "uv0", ref.UV0);
    GetImVec2Attrib(xml, "uv1", ref.UV1);
}
