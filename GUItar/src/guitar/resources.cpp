#include <iostream>
#include <guitar/resources.hpp>
#include <guitar/xml.hpp>
#include <tinyxml2/tinyxml2.h>

guitar::Resources::Resources(const std::filesystem::path &executable)
        : m_Root(executable.parent_path())
{
}

guitar::Resources::~Resources() = default;

void guitar::Resources::CheckErrors() const
{
    if (m_Root.empty())
        throw std::runtime_error("resource root is not initialized");
}

std::ifstream guitar::Resources::Open(const std::string &name) const
{
    CheckErrors();
    return {m_Root / name};
}

void guitar::Resources::Index()
{
    CheckErrors();
    IndexDirectory(m_Root);
}

void guitar::Resources::IndexDirectory(const std::filesystem::path &path)
{
    for (auto &entry: std::filesystem::directory_iterator(path))
    {
        if (entry.is_directory())
        {
            IndexDirectory(entry);
            continue;
        }

        IndexFile(entry);
    }
}

void guitar::Resources::IndexFile(const std::filesystem::path &path)
{
    if (!path.has_extension() || path.extension() != ".xml")
        return; // Not a layout

    std::cout << path << std::endl;
    tinyxml2::XMLDocument doc;
    doc.LoadFile(path.string().c_str());

    const auto root = doc.RootElement();

    const std::string type = root->Name();
    if (type == "layout") return ParseLayout(root);
    if (type == "app") return ParseApp(root);

    std::cerr << "[Resources] Failed to index " << path << ": undefined type '" << type << "'" << std::endl;
}

void guitar::Resources::ParseLayout(const tinyxml2::XMLElement *xml)
{
    Layout layout;
    FromXML(xml, layout);
    m_Layouts[layout.ID] = layout;
}

void guitar::Resources::ParseApp(const tinyxml2::XMLElement *xml)
{
    m_App = {};
    FromXML(xml, m_App);
}

guitar::AppConfig &guitar::Resources::GetApp()
{
    return m_App;
}

guitar::Layout *guitar::Resources::GetLayout(const std::string &id)
{
    return &m_Layouts[id];
}
