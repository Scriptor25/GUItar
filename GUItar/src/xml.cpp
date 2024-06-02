#include <iostream>
#include <guitar/layout.hpp>
#include <guitar/xml.hpp>

void guitar::GetStringAttrib(const tinyxml2::XMLElement* pXml, const std::string& name, std::string& ref)
{
    const char* value;
    if (pXml->QueryStringAttribute(name.c_str(), &value) == tinyxml2::XML_SUCCESS)
        ref = value;
}

void guitar::GetIntAttrib(const tinyxml2::XMLElement* pXml, const std::string& name, int& ref)
{
    pXml->QueryIntAttribute(name.c_str(), &ref);
}

void guitar::GetBoolAttrib(const tinyxml2::XMLElement* pXml, const std::string& name, bool& ref)
{
    pXml->QueryBoolAttribute(name.c_str(), &ref);
}

void guitar::GetImVec2Attrib(const tinyxml2::XMLElement* pXml, const std::string& name, ImVec2& ref)
{
    std::string value;
    GetStringAttrib(pXml, name, value);
    if (!value.empty())
    {
        const auto idx = value.find_first_of(';');
        ref.x = std::stof(value.substr(0, idx));
        ref.y = std::stof(value.substr(idx + 1));
    }
}

void guitar::FromXML(const tinyxml2::XMLElement* pXml, AppConfig& ref)
{
    GetStringAttrib(pXml, "title", ref.Title);
    GetStringAttrib(pXml, "version", ref.Version);
    GetStringAttrib(pXml, "layout", ref.Layout);
    GetIntAttrib(pXml, "width", ref.Width);
    GetIntAttrib(pXml, "height", ref.Height);
    GetBoolAttrib(pXml, "viewports", ref.Viewports);
}

void guitar::FromXML(const tinyxml2::XMLElement* pXml, Layout& ref)
{
    GetStringAttrib(pXml, "id", ref.ID);

    if (pXml->NoChildren())
        return;

    for (auto ptr = pXml->FirstChildElement(); ptr; ptr = ptr->NextSiblingElement())
    {
        Element* element = nullptr;
        FromXML(ptr, element);
        if (element)
            ref.Elements.push_back(element);
    }
}

void guitar::FromXML(const tinyxml2::XMLElement* pXml, Element*& ref)
{
    const std::string type = pXml->Name();
    if (type == "dockspace")
    {
        const auto element = new DockSpaceElement();
        FromXML(pXml, *element);
        ref = element;
        return;
    }
    if (type == "demo")
    {
        const auto element = new DemoElement();
        FromXML(pXml, *element);
        ref = element;
        return;
    }
    if (type == "window")
    {
        const auto element = new WindowElement();
        FromXML(pXml, *element);
        ref = element;
        return;
    }
    if (type == "button")
    {
        const auto element = new ButtonElement();
        FromXML(pXml, *element);
        ref = element;
        return;
    }
    if (type == "image")
    {
        const auto element = new ImageElement();
        FromXML(pXml, *element);
        ref = element;
        return;
    }

    if (const auto& func = TextElement::FUNCS[type])
    {
        const auto element = new TextElement();
        element->Func = func;
        element->Text = pXml->GetText();
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

void guitar::FromXML(const tinyxml2::XMLElement* pXml, WindowElement& ref)
{
    GetStringAttrib(pXml, "name", ref.Name);

    if (pXml->NoChildren())
        return;

    for (auto ptr = pXml->FirstChildElement(); ptr; ptr = ptr->NextSiblingElement())
    {
        Element* element = nullptr;
        FromXML(ptr, element);
        if (element)
            ref.Elements.push_back(element);
    }
}

void guitar::FromXML(const tinyxml2::XMLElement* pXml, ButtonElement& ref)
{
    ref.Text = pXml->GetText();
    GetStringAttrib(pXml, "action", ref.Action);
}

void guitar::FromXML(const tinyxml2::XMLElement* pXml, ImageElement& ref)
{
    GetStringAttrib(pXml, "src", ref.Source);
    GetStringAttrib(pXml, "type", ref.Type);
    GetBoolAttrib(pXml, "keepRatio", ref.KeepRatio);
    GetImVec2Attrib(pXml, "bounds", ref.Bounds);
    GetImVec2Attrib(pXml, "uv0", ref.UV0);
    GetImVec2Attrib(pXml, "uv1", ref.UV1);
}
