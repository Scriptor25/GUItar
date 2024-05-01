#pragma once

#include <filesystem>
#include <fstream>
#include <functional>
#include <guitar/layout.hpp>
#include <map>
#include <string>
#include "tinyxml2.h"

namespace guitar
{
    class ResourceManager
    {
    public:
        explicit ResourceManager(const std::filesystem::path &executable);

        void CheckErrors() const;

        [[nodiscard]] std::ifstream Open(const std::string &name) const;

        void Index();

        AppConfig &GetConfig();

        Layout *GetLayout(const std::string &id);

    private:
        void IndexDirectory(const std::filesystem::path &path);

        void IndexFile(const std::filesystem::path &path);

        void ParseLayout(const tinyxml2::XMLElement *xml);

        void ParseApp(const tinyxml2::XMLElement *xml);

    private:
        std::filesystem::path m_Root;

        bool m_AppConfigured = false;
        AppConfig m_App;

        std::map<std::string, Layout> m_Layouts;
    };
}
