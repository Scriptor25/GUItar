#pragma once

#include <string>
#include <GL/glew.h>
#include <guitar/guitar.hpp>

namespace guitar
{
    struct Image
    {
        Image();
        explicit Image(const std::string& filename);
        ~Image();

        void Load(const ResourceManager& resources);
        void StorePixels(int width, int height, const unsigned char* pixels) const;

        std::string Filename;
        GLuint Texture = 0;
        int Width = 0, Height = 0;
    };
}
