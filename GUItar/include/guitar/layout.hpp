#pragma once

#include <string>
#include <vector>
#include <guitar/guitar.hpp>

namespace guitar
{
    struct Layout
    {
        void Draw(EventManager &events);

        std::string ID;
        std::vector<Element *> Elements;
    };

    struct Element
    {
        virtual ~Element() = default;

        virtual void Draw(EventManager &events) = 0;
    };

    struct DockSpaceElement : Element
    {
        void Draw(EventManager &events) override;
    };

    struct DemoElement : Element
    {
        void Draw(EventManager &events) override;
    };

    struct WindowElement : Element
    {
        void Draw(EventManager &events) override;

        std::string Name;
        std::vector<Element *> Elements;
    };

    struct TextElement : Element
    {
        void Draw(EventManager &events) override;

        std::string Text;
    };

    struct ButtonElement : Element
    {
        void Draw(EventManager &events) override;

        std::string Text;
        std::string Action;
    };
}
