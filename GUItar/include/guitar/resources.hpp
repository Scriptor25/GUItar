#pragma once

#include <filesystem>
#include <fstream>
#include <map>
#include <string>
#include <tinyxml2.h>
#include <guitar/guitar.hpp>
#include <guitar/layout.hpp>

namespace guitar
{
    class ResourceManager
    {
    public:
        explicit ResourceManager(const std::filesystem::path& executable);

        void CheckErrors() const;

        [[nodiscard]] std::ifstream Open(const std::string& name, std::ios_base::openmode mode) const;

        void Index();

        AppConfig& GetConfig();
        const Layout& GetLayout(const std::string& id);

        std::shared_ptr<Image> GetImage(const std::string& id);

        void LoadAllImages();

    private:
        void IndexDirectory(const std::filesystem::path& path);
        void IndexFile(const std::filesystem::path& path);

        void ParseLayout(const tinyxml2::XMLElement* xml);
        void ParseApp(const tinyxml2::XMLElement* xml);
        void ParseImage(const tinyxml2::XMLElement* xml);

        std::filesystem::path m_Root;

        bool m_AppConfigured = false;
        AppConfig m_App;
        std::map<std::string, Layout> m_Layouts;
        std::map<std::string, std::shared_ptr<Image>> m_Images;
    };
}
