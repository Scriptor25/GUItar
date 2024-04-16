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
        Resources(const std::filesystem::path &root);
        ~Resources();

        void CheckErrors();
        std::ifstream Open(const std::string &name);

    private:
        std::filesystem::path m_Root;

        std::vector<Layout> m_Layouts;
    };
}
