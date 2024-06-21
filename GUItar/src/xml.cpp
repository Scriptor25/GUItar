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
    GetStringAttrib(pXml, "iniDirectory", ref.IniDirectory);
}

void guitar::FromXML(const tinyxml2::XMLElement* pXml, Layout& ref)
{
    GetStringAttrib(pXml, "id", ref.ID);
    GetBoolAttrib(pXml, "dockspace", ref.Dockspace);

    if (pXml->NoChildren())
        return;

    for (auto ptr = pXml->FirstChildElement(); ptr; ptr = ptr->NextSiblingElement())
        FromXML(ptr, ref.Elements.emplace_back());
}

void guitar::FromXML(const tinyxml2::XMLElement* pXml, Menu& ref)
{
    GetStringAttrib(pXml, "label", ref.Label);

    if (pXml->NoChildren())
        return;

    for (auto ptr = pXml->FirstChildElement(); ptr; ptr = ptr->NextSiblingElement())
        FromXML(ptr, ref.Elements.emplace_back());
}

template <typename T>
static void from_xml(const tinyxml2::XMLElement* pXml, std::unique_ptr<guitar::Element>& ref)
{
    auto element = std::make_unique<T>();
    guitar::FromXML(pXml, *element);
    ref = std::move(element);
}

static std::map<std::string, std::function<void(const tinyxml2::XMLElement*, std::unique_ptr<guitar::Element>&)>> xml_funcs
{
    {"bullet", from_xml<guitar::SimpleElement>},
    {"button", from_xml<guitar::ButtonElement>},
    {"checkbox", from_xml<guitar::CheckboxElement>},
    {"combo", from_xml<guitar::ComboElement>},
    {"custom", from_xml<guitar::CustomElement>},
    {"demo", from_xml<guitar::DemoElement>},
    {"image", from_xml<guitar::ImageElement>},
    {"inputText", from_xml<guitar::InputTextElement>},
    {"item", from_xml<guitar::MenuItemElement>},
    {"menubar", from_xml<guitar::MenuBarElement>},
    {"newline", from_xml<guitar::SimpleElement>},
    {"separator", from_xml<guitar::SimpleElement>},
    {"separatorText", from_xml<guitar::TextElement>},
    {"spacing", from_xml<guitar::SimpleElement>},
    {"text", from_xml<guitar::TextElement>},
    {"window", from_xml<guitar::WindowElement>},
};

void guitar::FromXML(const tinyxml2::XMLElement* pXml, std::unique_ptr<Element>& ref)
{
    const std::string type = pXml->Name();
    if (const auto& func = xml_funcs[type])
        return func(pXml, ref);

    std::cerr << "[FromXML] Undefined element type '" << type << "'" << std::endl;
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

void guitar::FromXML(const tinyxml2::XMLElement* pXml, CustomElement& ref)
{
    GetStringAttrib(pXml, "event", ref.Event);
}

void guitar::FromXML(const tinyxml2::XMLElement* pXml, InputTextElement& ref)
{
    GetStringAttrib(pXml, "label", ref.Label);
    GetStringAttrib(pXml, "hint", ref.Hint);
    GetStringAttrib(pXml, "event", ref.Event);
    GetStringAttrib(pXml, "default", ref.Default);
}

void guitar::FromXML(const tinyxml2::XMLElement* pXml, MenuItemElement& ref)
{
    ref.Label = pXml->GetText();
    GetStringAttrib(pXml, "event", ref.Event);
    std::string shortcut;
    GetStringAttrib(pXml, "shortcut", shortcut);
    ref.Shortcut = KeyShortcut(shortcut);
}

void guitar::FromXML(const tinyxml2::XMLElement* pXml, MenuBarElement& ref)
{
    if (pXml->NoChildren())
        return;

    for (auto ptr = pXml->FirstChildElement(); ptr; ptr = ptr->NextSiblingElement())
        FromXML(ptr, ref.Menus.emplace_back());
}

void guitar::FromXML(const tinyxml2::XMLElement* pXml, CheckboxElement& ref)
{
    GetStringAttrib(pXml, "label", ref.Label);
    GetStringAttrib(pXml, "event", ref.Event);
    GetStringAttrib(pXml, "default", ref.Default);
}

void guitar::FromXML(const tinyxml2::XMLElement* pXml, TextElement& ref)
{
    const std::string type = pXml->Name();
    const auto& func = TextElement::FUNCS[type];

    ref.Func = func;
    ref.Text = pXml->GetText();
}

void guitar::FromXML(const tinyxml2::XMLElement* pXml, SimpleElement& ref)
{
    const std::string type = pXml->Name();
    const auto& func = SimpleElement::FUNCS[type];

    ref.Func = func;
}
