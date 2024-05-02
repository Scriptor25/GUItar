#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <vector>
#include <guitar/image.hpp>
#include <guitar/resources.hpp>

guitar::Image::Image(const std::string& filename)
    : Filename(filename)
{
}

guitar::Image::~Image()
{
    glDeleteTextures(1, &Texture);
}

void guitar::Image::Load(const ResourceManager& resources)
{
    auto stream = resources.Open(Filename, std::ios_base::binary | std::ios_base::ate);
    if (!stream.is_open())
        return;

    std::vector<stbi_uc> buffer(stream.tellg());
    stream.seekg(0);
    stream.read(reinterpret_cast<char*>(buffer.data()), static_cast<long>(buffer.size()));
    stream.close();

    const auto pixels = stbi_load_from_memory(buffer.data(), static_cast<int>(buffer.size()), &Width, &Height, nullptr, 4);
    if (!pixels)
        return;

    glGenTextures(1, &Texture);
    glBindTexture(GL_TEXTURE_2D, Texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(pixels);
}
