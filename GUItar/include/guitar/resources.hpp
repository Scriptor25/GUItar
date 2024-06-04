#pragma once

#include <filesystem>
#include <fstream>
#include <map>
#include <string>
#include <tinyxml2.h>
#include <guitar/guitar.hpp>
#include <guitar/image.hpp>
#include <guitar/layout.hpp>

namespace guitar
{
    class ResourceManager
    {
    public:
        explicit ResourceManager(const std::filesystem::path& executable);

        [[nodiscard]] std::ifstream Open(const std::string& name, std::ios_base::openmode mode) const;

        void LoadAllImages();
        void Index();

        AppConfig& GetConfig();
        const Layout* GetLayout(const std::string& id);
        const Image* GetImage(const std::string& id);

        std::string GetString(EventManager& events, const std::string& str) const;

    private:
        void IndexDirectory(const std::filesystem::path& path);
        void IndexFile(const std::filesystem::path& path);

        void ParseLayout(const tinyxml2::XMLElement* pXml);
        void ParseApp(const tinyxml2::XMLElement* pXml);
        void ParseImage(const tinyxml2::XMLElement* pXml);

        std::filesystem::path m_Root;

        bool m_AppConfigured = false;
        AppConfig m_App;
        std::map<std::string, Layout> m_Layouts;
        std::map<std::string, Image> m_Images;
    };
}
