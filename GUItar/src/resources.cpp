#include <iostream>
#include <queue>
#include <tinyxml2.h>
#include <guitar/events.hpp>
#include <guitar/layout.hpp>
#include <guitar/resources.hpp>
#include <guitar/xml.hpp>

guitar::ResourceManager::ResourceManager(const std::filesystem::path& executable)
    : m_Root(executable.parent_path())
{
}

std::ifstream guitar::ResourceManager::Open(const std::string& name, const std::ios_base::openmode mode) const
{
    return std::move(std::ifstream(m_Root / name, std::ios_base::in | mode));
}

void guitar::ResourceManager::LoadAllImages()
{
    for (auto& [key_, image_] : m_Images)
        image_.Load(*this);
}

void guitar::ResourceManager::Index()
{
    IndexDirectory(m_Root);
}

void guitar::ResourceManager::IndexDirectory(const std::filesystem::path& path)
{
    std::queue<std::filesystem::directory_iterator> next;
    next.emplace(path);

    while (!next.empty())
    {
        auto directory = next.front();
        next.pop();

        for (const auto& entry : directory)
        {
            if (entry.is_directory())
            {
                next.emplace(entry);
                continue;
            }

            IndexFile(entry);
        }
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

void guitar::ResourceManager::ParseLayout(const tinyxml2::XMLElement* xml)
{
    Layout layout;
    FromXML(xml, layout);
    m_Layouts[layout.ID] = std::move(layout);
}

void guitar::ResourceManager::ParseApp(const tinyxml2::XMLElement* xml)
{
    if (m_AppConfigured)
        std::cerr << "[ResourceManager] Warning: app already configured" << std::endl;
    m_AppConfigured = true;
    FromXML(xml, m_App);
}

void guitar::ResourceManager::ParseImage(const tinyxml2::XMLElement* xml)
{
    std::string id, src, type;
    GetStringAttrib(xml, "id", id);
    GetStringAttrib(xml, "src", src);
    GetStringAttrib(xml, "type", type);

    m_Images[id] = Image(src);
}

guitar::AppConfig& guitar::ResourceManager::GetConfig()
{
    return m_App;
}

const guitar::Layout* guitar::ResourceManager::GetLayout(const std::string& id)
{
    return &m_Layouts[id];
}

const guitar::Image* guitar::ResourceManager::GetImage(const std::string& id)
{
    return &m_Images[id];
}

std::string guitar::ResourceManager::GetString(EventManager& events, const std::string& str) const
{
    if (str[0] == '$')
    {
        std::string result;
        const MutableEvent payload(this, result);
        events.Invoke(str.substr(1), &payload);
        return result;
    }

    return str;
}
