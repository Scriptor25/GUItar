#pragma once

#include <string>
#include <vector>
#include <guitar/guitar.hpp>

namespace guitar
{
    struct Layout
    {
        Layout();

        explicit Layout(ResourceManager *resources);

        void Draw();

        ResourceManager *Resources;
        std::string ID;
        std::vector<Element *> Elements;
    };

    struct Element
    {
        explicit Element(ResourceManager *resources);

        virtual ~Element() = default;

        virtual void Draw() = 0;

        ResourceManager *Resources;
    };

    struct DockSpaceElement : Element
    {
        explicit DockSpaceElement(ResourceManager *resources);

        void Draw() override;
    };

    struct DemoElement : Element
    {
        explicit DemoElement(ResourceManager *resources);

        void Draw() override;
    };

    struct WindowElement : Element
    {
        explicit WindowElement(ResourceManager *resources);

        void Draw() override;

        std::string Name;
        std::vector<Element *> Elements;
    };

    struct TextElement : Element
    {
        explicit TextElement(ResourceManager *resources);

        void Draw() override;

        std::string Text;
    };

    struct ButtonElement : Element
    {
        explicit ButtonElement(ResourceManager *resources);

        void Draw() override;

        std::string Text;
        std::string Action;
    };
}
