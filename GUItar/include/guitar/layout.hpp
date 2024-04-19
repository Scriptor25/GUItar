#pragma once

#include <string>
#include <vector>
#include <guitar/guitar.hpp>

namespace guitar
{
    struct Layout
    {
        void Draw();

        std::string ID;
        std::vector<Element *> Elements;
    };

    struct Element
    {
        virtual ~Element() = default;

        virtual void Draw() = 0;
    };

    struct DockSpaceElement : Element
    {
        void Draw() override;
    };

    struct DemoElement : Element
    {
        void Draw() override;
    };

    struct WindowElement : Element
    {
        void Draw() override;

        std::string Name;
        std::vector<Element *> Elements;
    };

    struct TextElement : Element
    {
        void Draw() override;

        std::string Text;
    };
}
