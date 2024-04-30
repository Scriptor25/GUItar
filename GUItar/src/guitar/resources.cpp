#include <iostream>
#include <guitar/resources.hpp>
#include <guitar/xml.hpp>
#include <tinyxml2/tinyxml2.h>

guitar::ResourceManager::ResourceManager(const std::filesystem::path &executable)
        : m_Root(executable.parent_path())
{
}

guitar::ResourceManager::~ResourceManager() = default;

void guitar::ResourceManager::CheckErrors() const
{
    if (m_Root.empty())
        throw std::runtime_error("resource root is not initialized");
}

std::ifstream guitar::ResourceManager::Open(const std::string &name) const
{
    CheckErrors();
    return {m_Root / name};
}

void guitar::ResourceManager::Index()
{
    CheckErrors();
    IndexDirectory(m_Root);
}

void guitar::ResourceManager::IndexDirectory(const std::filesystem::path &path)
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

void guitar::ResourceManager::IndexFile(const std::filesystem::path &path)
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

    std::cerr << "[ResourceManager] Failed to index " << path << ": undefined type '" << type << "'" << std::endl;
}

void guitar::ResourceManager::ParseLayout(const tinyxml2::XMLElement *xml)
{
    Layout layout;
    FromXML(xml, layout);
    m_Layouts[layout.ID] = layout;
}

void guitar::ResourceManager::ParseApp(const tinyxml2::XMLElement *xml)
{
    if (m_AppConfigured)
        std::cerr << "[ResourceManager] Warning: app already configured" << std::endl;
    m_AppConfigured = true;
    FromXML(xml, m_App);
}

guitar::AppConfig &guitar::ResourceManager::GetApp()
{
    return m_App;
}

guitar::Layout *guitar::ResourceManager::GetLayout(const std::string &id)
{
    return &m_Layouts[id];
}
