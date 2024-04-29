#pragma once

#include <filesystem>
#include <fstream>
#include <map>
#include <string>
#include <guitar/layout.hpp>
#include <tinyxml2/tinyxml2.h>

namespace guitar
{
    class Resources
    {
    public:
        explicit Resources(const std::filesystem::path& executable);

        ~Resources();

        void CheckErrors() const;

        [[nodiscard]]
        std::ifstream Open(const std::string& name) const;

        void Index();

        void IndexDirectory(const std::filesystem::path& path);

        void IndexFile(const std::filesystem::path& path);

        void ParseLayout(const tinyxml2::XMLElement* xml);

        void ParseApp(const tinyxml2::XMLElement* xml);

        AppConfig& GetApp();

        Layout* GetLayout(const std::string& id);

    private:
        std::filesystem::path m_Root;
        AppConfig m_App;

        std::map<std::string, Layout> m_Layouts;
    };
}
