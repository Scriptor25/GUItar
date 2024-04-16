#include <guitar/resources.hpp>
#include <iostream>

guitar::Resources::Resources(const std::filesystem::path& executable)
    : m_Root(executable.parent_path())
{
}

guitar::Resources::~Resources() = default;

void guitar::Resources::CheckErrors() const
{
    if (m_Root.empty())
        throw std::runtime_error("resource root is not initialized");
}

std::ifstream guitar::Resources::Open(const std::string& name) const
{
    CheckErrors();
    return {m_Root / name};
}

void guitar::Resources::Index()
{
    CheckErrors();
    IndexDirectory(m_Root);
}

void guitar::Resources::IndexDirectory(const std::filesystem::path& path)
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

void guitar::Resources::IndexFile(const std::filesystem::path& path)
{
    if (!path.has_extension() || path.extension() != ".xml")
        return; // Not a layout

    std::cout << path << std::endl;
}
