#pragma once

#include <filesystem>
#include <fstream>
#include <guitar/guitar.hpp>
#include <guitar/layout.hpp>
#include <string>
#include <vector>

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

    private:
        std::filesystem::path m_Root;

        std::vector<Layout> m_Layouts;
    };
}
