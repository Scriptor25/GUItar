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
    GetBoolAttrib(xml, "viewports", ref.Viewports);
    GetStringAttrib(xml, "iniDirectory", ref.IniDirectory);
}

void guitar::FromXML(const tinyxml2::XMLElement* xml, Layout& ref)
{
    GetStringAttrib(xml, "id", ref.ID);
    GetBoolAttrib(xml, "dockspace", ref.Dockspace);

    if (xml->NoChildren())
        return;

    for (auto ptr = xml->FirstChildElement(); ptr; ptr = ptr->NextSiblingElement())
        FromXML(ptr, ref.Elements.emplace_back());
}

void guitar::FromXML(const tinyxml2::XMLElement* xml, Menu& ref)
{
    GetStringAttrib(xml, "label", ref.Label);

    if (xml->NoChildren())
        return;

    for (auto ptr = xml->FirstChildElement(); ptr; ptr = ptr->NextSiblingElement())
        FromXML(ptr, ref.Elements.emplace_back());
}

template <typename T>
static void from_xml(const tinyxml2::XMLElement* pXml, std::unique_ptr<guitar::Element>& ref)
{
    auto element = std::make_unique<T>();
    guitar::FromXML(pXml, *element);
    ref = std::move(element);
}

static std::map<std::string, std::function<void(const tinyxml2::XMLElement*, std::unique_ptr<guitar::Element>&)>>
xml_funcs
{
    {"bullet", from_xml<guitar::SimpleElement>},
    {"button", from_xml<guitar::ButtonElement>},
    {"checkbox", from_xml<guitar::CheckboxElement>},
    {"combo", from_xml<guitar::ComboElement>},
    {"custom", from_xml<guitar::CustomElement>},
    {"demo", from_xml<guitar::DemoElement>},
    {"group", from_xml<guitar::GroupElement>},
    {"image", from_xml<guitar::ImageElement>},
    {"inputText", from_xml<guitar::InputTextElement>},
    {"item", from_xml<guitar::MenuItemElement>},
    {"menubar", from_xml<guitar::MenuBarElement>},
    {"newline", from_xml<guitar::SimpleElement>},
    {"sameline", from_xml<guitar::SimpleElement>},
    {"separator", from_xml<guitar::SimpleElement>},
    {"separatorText", from_xml<guitar::TextElement>},
    {"spacing", from_xml<guitar::SimpleElement>},
    {"text", from_xml<guitar::TextElement>},
    {"window", from_xml<guitar::WindowElement>},
};

void guitar::FromXML(const tinyxml2::XMLElement* xml, std::unique_ptr<Element>& ref)
{
    const std::string type = xml->Name();
    if (const auto& func = xml_funcs[type])
        return func(xml, ref);

    std::cerr << "[FromXML] Undefined element type '" << type << "'" << std::endl;
}

void guitar::FromXML(const tinyxml2::XMLElement*, DemoElement&)
{
}

void guitar::FromXML(const tinyxml2::XMLElement* xml, GroupElement& ref)
{
    if (xml->NoChildren())
        return;

    for (auto ptr = xml->FirstChildElement(); ptr; ptr = ptr->NextSiblingElement())
    {
        std::unique_ptr<Element> element;
        FromXML(ptr, element);
        if (element)
            ref.Elements.push_back(std::move(element));
    }
}

void guitar::FromXML(const tinyxml2::XMLElement* xml, WindowElement& ref)
{
    GetStringAttrib(xml, "name", ref.Name);

    if (xml->NoChildren())
        return;

    for (auto ptr = xml->FirstChildElement(); ptr; ptr = ptr->NextSiblingElement())
    {
        std::unique_ptr<Element> element;
        FromXML(ptr, element);
        if (element)
            ref.Elements.push_back(std::move(element));
    }
}

void guitar::FromXML(const tinyxml2::XMLElement* xml, ButtonElement& ref)
{
    ref.Text = xml->GetText();
    GetStringAttrib(xml, "event", ref.Event);
}

void guitar::FromXML(const tinyxml2::XMLElement* xml, ImageElement& ref)
{
    GetStringAttrib(xml, "src", ref.Source);
    GetBoolAttrib(xml, "keepRatio", ref.KeepRatio);
    GetImVec2Attrib(xml, "bounds", ref.Bounds);
    GetImVec2Attrib(xml, "uv0", ref.UV0);
    GetImVec2Attrib(xml, "uv1", ref.UV1);
}

void guitar::FromXML(const tinyxml2::XMLElement* xml, ComboElement& ref)
{
    GetStringAttrib(xml, "label", ref.Label);
    GetStringAttrib(xml, "preview", ref.Preview);

    if (xml->NoChildren())
        return;

    for (auto ptr = xml->FirstChildElement(); ptr; ptr = ptr->NextSiblingElement())
    {
        std::unique_ptr<Element> element;
        FromXML(ptr, element);
        if (element)
            ref.Elements.push_back(std::move(element));
    }
}

void guitar::FromXML(const tinyxml2::XMLElement* xml, CustomElement& ref)
{
    GetStringAttrib(xml, "event", ref.Event);
}

void guitar::FromXML(const tinyxml2::XMLElement* xml, InputTextElement& ref)
{
    GetStringAttrib(xml, "label", ref.Label);
    GetStringAttrib(xml, "hint", ref.Hint);
    GetStringAttrib(xml, "event", ref.Event);
    GetStringAttrib(xml, "default", ref.Default);
}

void guitar::FromXML(const tinyxml2::XMLElement* xml, MenuItemElement& ref)
{
    ref.Label = xml->GetText();
    GetStringAttrib(xml, "event", ref.Event);
    std::string shortcut;
    GetStringAttrib(xml, "shortcut", shortcut);
    ref.Shortcut = KeyShortcut(shortcut);
}

void guitar::FromXML(const tinyxml2::XMLElement* xml, MenuBarElement& ref)
{
    if (xml->NoChildren())
        return;

    for (auto ptr = xml->FirstChildElement(); ptr; ptr = ptr->NextSiblingElement())
        FromXML(ptr, ref.Menus.emplace_back());
}

void guitar::FromXML(const tinyxml2::XMLElement* xml, CheckboxElement& ref)
{
    GetStringAttrib(xml, "label", ref.Label);
    GetStringAttrib(xml, "event", ref.Event);
    GetStringAttrib(xml, "default", ref.Default);
}

void guitar::FromXML(const tinyxml2::XMLElement* xml, TextElement& ref)
{
    const std::string type = xml->Name();
    const auto& func = TextElement::FUNCS[type];

    ref.Func = func;
    ref.Text = xml->GetText();
}

void guitar::FromXML(const tinyxml2::XMLElement* xml, SimpleElement& ref)
{
    const std::string type = xml->Name();
    const auto& func = SimpleElement::FUNCS[type];

    ref.Func = func;
}
