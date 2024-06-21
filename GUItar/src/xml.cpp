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
        std::unique_ptr<Element> element;
        FromXML(ptr, element);
        if (element)
            ref.Elements.push_back(std::move(element));
    }
}

void guitar::FromXML(const tinyxml2::XMLElement* pXml, std::unique_ptr<Element>& ref)
{
    const std::string type = pXml->Name();
    if (type == "dockspace")
    {
        auto element = std::make_unique<DockSpaceElement>();
        FromXML(pXml, *element);
        ref = std::move(element);
        return;
    }
    if (type == "demo")
    {
        auto element = std::make_unique<DemoElement>();
        FromXML(pXml, *element);
        ref = std::move(element);
        return;
    }
    if (type == "window")
    {
        auto element = std::make_unique<WindowElement>();
        FromXML(pXml, *element);
        ref = std::move(element);
        return;
    }
    if (type == "button")
    {
        auto element = std::make_unique<ButtonElement>();
        FromXML(pXml, *element);
        ref = std::move(element);
        return;
    }
    if (type == "image")
    {
        auto element = std::make_unique<ImageElement>();
        FromXML(pXml, *element);
        ref = std::move(element);
        return;
    }
    if (type == "combo")
    {
        auto element = std::make_unique<ComboElement>();
        FromXML(pXml, *element);
        ref = std::move(element);
        return;
    }
    if (type == "custom")
    {
        auto element = std::make_unique<CustomElement>();
        FromXML(pXml, *element);
        ref = std::move(element);
        return;
    }
    if (type == "inputText")
    {
        auto element = std::make_unique<InputTextElement>();
        FromXML(pXml, *element);
        ref = std::move(element);
        return;
    }

    if (const auto& func = TextElement::FUNCS[type])
    {
        auto element = std::make_unique<TextElement>();
        element->Func = func;
        element->Text = pXml->GetText();
        ref = std::move(element);
        return;
    }

    if (const auto& func = SimpleElement::FUNCS[type])
    {
        auto element = std::make_unique<SimpleElement>();
        element->Func = func;
        ref = std::move(element);
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
        std::unique_ptr<Element> element;
        FromXML(ptr, element);
        if (element)
            ref.Elements.push_back(std::move(element));
    }
}

void guitar::FromXML(const tinyxml2::XMLElement* pXml, ButtonElement& ref)
{
    ref.Text = pXml->GetText();
    GetStringAttrib(pXml, "event", ref.Event);
}

void guitar::FromXML(const tinyxml2::XMLElement* pXml, ImageElement& ref)
{
    GetStringAttrib(pXml, "src", ref.Source);
    GetBoolAttrib(pXml, "keepRatio", ref.KeepRatio);
    GetImVec2Attrib(pXml, "bounds", ref.Bounds);
    GetImVec2Attrib(pXml, "uv0", ref.UV0);
    GetImVec2Attrib(pXml, "uv1", ref.UV1);
}

void guitar::FromXML(const tinyxml2::XMLElement* pXml, ComboElement& ref)
{
    GetStringAttrib(pXml, "label", ref.Label);
    GetStringAttrib(pXml, "preview", ref.Preview);

    for (auto ptr = pXml->FirstChildElement(); ptr; ptr = ptr->NextSiblingElement())
    {
        std::unique_ptr<Element> element;
        FromXML(ptr, element);
        if (element)
            ref.Elements.push_back(std::move(element));
    }
}

void guitar::FromXML(const tinyxml2::XMLElement* pXml, CustomElement& ref)
{
    GetStringAttrib(pXml, "event", ref.Event);
}

void guitar::FromXML(const tinyxml2::XMLElement* pXml, InputTextElement& ref)
{
    GetStringAttrib(pXml, "label", ref.Label);
    GetStringAttrib(pXml, "hint", ref.Hint);
    GetStringAttrib(pXml, "event", ref.Event);
}
