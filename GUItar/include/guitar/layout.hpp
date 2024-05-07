#pragma once

#include <functional>
#include <imgui.h>
#include <map>
#include <string>
#include <vector>
#include <guitar/guitar.hpp>

namespace guitar
{
    struct Layout
    {
        void Draw(ResourceManager& resources, EventManager& events) const;

        std::string ID;
        std::vector<Element*> Elements;
    };

    struct Element
    {
        virtual ~Element() = default;

        virtual void Draw(ResourceManager& resources, EventManager& events) = 0;
    };

    struct DockSpaceElement : Element
    {
        void Draw(ResourceManager& resources, EventManager& events) override;
    };

    struct DemoElement : Element
    {
        void Draw(ResourceManager& resources, EventManager& events) override;
    };

    struct WindowElement : Element
    {
        void Draw(ResourceManager& resources, EventManager& events) override;

        std::string Name;
        std::vector<Element*> Elements;
    };

    struct ButtonElement : Element
    {
        void Draw(ResourceManager& resources, EventManager& events) override;

        std::string Text;
        std::string Action;
    };

    struct ImageElement : Element
    {
        void Draw(ResourceManager& resources, EventManager& events) override;

        std::string Source, Type;
        bool KeepRatio = true;
        ImVec2 Bounds{-1, -1}, UV0{0, 0}, UV1{1, 1};
    };

    struct TextElement : Element
    {
        void Draw(ResourceManager& resources, EventManager& events) override;

        static std::map<std::string, std::function<void(const char*)>> FUNCS;

        std::string Text;
        std::function<void(const char*)> Func;
    };

    struct SimpleElement : Element
    {
        void Draw(ResourceManager& resources, EventManager& events) override;

        static std::map<std::string, std::function<void()>> FUNCS;

        std::function<void()> Func;
    };
}
