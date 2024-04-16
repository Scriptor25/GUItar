#include <guitar/resources.hpp>

guitar::Resources::Resources(const std::filesystem::path &root)
    : m_Root(root)
{
}

guitar::Resources::~Resources()
{
}

void guitar::Resources::CheckErrors()
{
    if (m_Root.empty())
        throw std::runtime_error("resource root is not initialized");
}

std::ifstream guitar::Resources::Open(const std::string &name)
{
    CheckErrors();
    return std::ifstream(m_Root / name);
}
