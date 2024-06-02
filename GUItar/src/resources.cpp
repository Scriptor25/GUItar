#include <iostream>
#include <tinyxml2.h>
#include <guitar/image.hpp>
#include <guitar/layout.hpp>
#include <guitar/resources.hpp>
#include <guitar/xml.hpp>

guitar::ResourceManager::ResourceManager(const std::filesystem::path& executable)
    : m_Root(executable.parent_path())
{
}

void guitar::ResourceManager::CheckErrors() const
{
    if (m_Root.empty())
        throw std::runtime_error("resource root is not initialized");
}

std::ifstream guitar::ResourceManager::Open(const std::string& name, const std::ios_base::openmode mode) const
{
    CheckErrors();
    return {m_Root / name, std::ios_base::in | mode};
}

void guitar::ResourceManager::Index()
{
    CheckErrors();
    IndexDirectory(m_Root);
}

void guitar::ResourceManager::IndexDirectory(const std::filesystem::path& path)
{
    for (auto& entry : std::filesystem::directory_iterator(path))
    {
        if (entry.is_directory())
        {
            IndexDirectory(entry);
            continue;
        }

        IndexFile(entry);
    }
}

void guitar::ResourceManager::IndexFile(const std::filesystem::path& path)
{
    if (!path.has_extension() || path.extension() != ".xml")
        return;

    std::cout << path << std::endl;
    tinyxml2::XMLDocument doc;
    doc.LoadFile(path.string().c_str());

    const auto root = doc.RootElement();

    const std::string type = root->Name();
    if (type == "layout") return ParseLayout(root);
    if (type == "app") return ParseApp(root);
    if (type == "image") return ParseImage(root);

    std::cerr << "[ResourceManager] Failed to index " << path << ": undefined xml type '" << type << "'" << std::endl;
}

void guitar::ResourceManager::ParseLayout(const tinyxml2::XMLElement* pXml)
{
    Layout layout;
    FromXML(pXml, layout);
    m_Layouts[layout.ID] = layout;
}

void guitar::ResourceManager::ParseApp(const tinyxml2::XMLElement* pXml)
{
    if (m_AppConfigured)
        std::cerr << "[ResourceManager] Warning: app already configured" << std::endl;
    m_AppConfigured = true;
    FromXML(pXml, m_App);
}

void guitar::ResourceManager::ParseImage(const tinyxml2::XMLElement* pXml)
{
    std::string id, src, type;
    GetStringAttrib(pXml, "id", id);
    GetStringAttrib(pXml, "src", src);
    GetStringAttrib(pXml, "type", type); // TODO: differentiate between bitmap and vector images

    m_Images[id] = std::make_shared<Image>(src);
}

guitar::AppConfig& guitar::ResourceManager::GetConfig()
{
    return m_App;
}

const guitar::Layout& guitar::ResourceManager::GetLayout(const std::string& id)
{
    return m_Layouts[id];
}

std::shared_ptr<guitar::Image> guitar::ResourceManager::GetImage(const std::string& id)
{
    return m_Images[id];
}

void guitar::ResourceManager::LoadAllImages()
{
    for (const auto& [key, image] : m_Images)
        image->Load(*this);
}
