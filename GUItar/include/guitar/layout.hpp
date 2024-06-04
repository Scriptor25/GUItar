#pragma once

#include <functional>
#include <imgui.h>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <guitar/guitar.hpp>

namespace guitar
{
    struct Element
    {
        virtual ~Element();

        virtual void Draw(ResourceManager& resources, EventManager& events) = 0;
    };

    struct Layout
    {
        Layout();

        Layout(const Layout&) = delete;
        Layout& operator=(const Layout&) = delete;
        Layout& operator=(Layout&&) noexcept;

        void Draw(ResourceManager& resources, EventManager& events) const;

        std::string ID;
        std::vector<std::unique_ptr<Element>> Elements;
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
        std::vector<std::unique_ptr<Element>> Elements;
    };

    struct ButtonElement : Element
    {
        void Draw(ResourceManager& resources, EventManager& events) override;

        std::string Text;
        std::string Event;
    };

    struct ImageElement : Element
    {
        void Draw(ResourceManager& resources, EventManager& events) override;

        std::string Source;
        bool KeepRatio = true;
        ImVec2 Bounds{-1, -1}, UV0{0, 0}, UV1{1, 1};
    };

    typedef std::function<void(const char*)> TextFunc;

    struct TextElement : Element
    {
        void Draw(ResourceManager& resources, EventManager& events) override;

        static std::map<std::string, TextFunc> FUNCS;

        std::string Text;
        TextFunc Func;
    };

    struct SimpleElement : Element
    {
        void Draw(ResourceManager& resources, EventManager& events) override;

        static std::map<std::string, std::function<void()>> FUNCS;

        std::function<void()> Func;
    };

    struct ComboElement : Element
    {
        void Draw(ResourceManager& resources, EventManager& events) override;

        std::string Label;
        std::string Preview;
        std::vector<std::unique_ptr<Element>> Elements;
    };

    struct CustomElement : Element
    {
        void Draw(ResourceManager& resources, EventManager& events) override;

        std::string Event;
    };
}
