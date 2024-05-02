#pragma once

#include <string>
#include <GL/glew.h>
#include <guitar/guitar.hpp>

namespace guitar
{
    struct Image
    {
        explicit Image(const std::string& filename);
        ~Image();

        void Load(const ResourceManager& resources);

        std::string Filename;
        GLuint Texture = 0;
        int Width = 0, Height = 0;
    };
}
