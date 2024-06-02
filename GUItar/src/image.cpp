#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <stb_image.h>
#include <vector>
#include <guitar/image.hpp>
#include <guitar/resources.hpp>

guitar::Image::Image()
    : Image(std::string())
{
}

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
    if (Filename.empty())
    {
        std::cerr << "[Image] Cannot load image from empty filename" << std::endl;
        return;
    }

    auto stream = resources.Open(Filename, std::ios_base::binary | std::ios_base::ate);
    if (!stream.is_open())
    {
        std::cerr << "[Image] Failed to open image from '" << Filename << "'" << std::endl;
        return;
    }

    std::vector<stbi_uc> buffer(stream.tellg());
    stream.seekg(0);
    stream.read(reinterpret_cast<char*>(buffer.data()), static_cast<std::streamsize>(buffer.size()));
    stream.close();

    int width, height;
    const auto pixels = stbi_load_from_memory(buffer.data(), static_cast<int>(buffer.size()), &width, &height, nullptr, 4);
    if (!pixels)
    {
        std::cerr << "[Image] Failed to load image from '" << Filename << "' with stbi: " << stbi_failure_reason() << std::endl;
        return;
    }

    StorePixels(width, height, pixels);

    stbi_image_free(pixels);
}

void guitar::Image::StorePixels(const int width, const int height, const unsigned char* pixels)
{
    Width = width;
    Height = height;

    if (!Texture)
    {
        glGenTextures(1, &Texture);
        if (!Texture)
        {
            std::cerr << "[Image] Failed to create GL texture" << std::endl;
            return;
        }

        glBindTexture(GL_TEXTURE_2D, Texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    glBindTexture(GL_TEXTURE_2D, Texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
}
