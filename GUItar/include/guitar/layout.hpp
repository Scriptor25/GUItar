#pragma once

#include <functional>
#include <imgui.h>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <guitar/events.hpp>
#include <guitar/guitar.hpp>

namespace guitar
{
    struct Element
    {
        virtual ~Element();

        virtual void Register(ResourceManager& resources, EventManager& events);
        virtual void Release(ResourceManager& resources, EventManager& events);

        virtual void Draw(ResourceManager& resources, EventManager& events) = 0;
    };

    struct Layout
    {
        Layout();

        Layout(const Layout&) = delete;
        Layout& operator=(const Layout&) = delete;
        Layout& operator=(Layout&&) noexcept;

        void Register(ResourceManager& resources, EventManager& events) const;
        void Release(ResourceManager& resources, EventManager& events) const;

        void Draw(ResourceManager& resources, EventManager& events) const;

        std::string ID;
        bool Dockspace = false;
        std::vector<std::unique_ptr<Element>> Elements;
    };

    struct DemoElement : Element
    {
        void Draw(ResourceManager& resources, EventManager& events) override;
    };

    struct WindowElement : Element
    {
        void Register(ResourceManager& resources, EventManager& events) override;
        void Release(ResourceManager& resources, EventManager& events) override;

        void Draw(ResourceManager& resources, EventManager& events) override;

        std::string Name;
        std::vector<std::unique_ptr<Element>> Elements;
    };

    struct GroupElement : Element
    {
        void Register(ResourceManager& resources, EventManager& events) override;
        void Release(ResourceManager& resources, EventManager& events) override;

        void Draw(ResourceManager& resources, EventManager& events) override;

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
        void Register(ResourceManager& resources, EventManager& events) override;
        void Release(ResourceManager& resources, EventManager& events) override;

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

    struct InputTextElement : Element
    {
        void Register(ResourceManager& resources, EventManager& events) override;

        void Draw(ResourceManager& resources, EventManager& events) override;

        std::string Label;
        std::string Hint;
        std::string Event;
        std::string Default;

        std::string Var;
    };

    struct ShortcutInfo
    {
        int Key = 0;
        bool Ctrl = false;
        bool Alt = false;
        bool Super = false;
        bool Shift = false;
    };

    struct KeyShortcut
    {
        KeyShortcut();
        explicit KeyShortcut(const std::string&);

        [[nodiscard]] bool Matches(const ImmutableEvent<KeyPayload>& event) const;
        [[nodiscard]] std::string String() const;

        std::vector<ShortcutInfo> Infos;
    };

    struct MenuItemElement : Element
    {
        void Register(ResourceManager& resources, EventManager& events) override;
        void Release(ResourceManager& resources, EventManager& events) override;

        void Draw(ResourceManager& resources, EventManager& events) override;

        std::string Label;
        std::string Event;
        KeyShortcut Shortcut;
    };

    struct Menu
    {
        void Register(ResourceManager& resources, EventManager& events) const;
        void Release(ResourceManager& resources, EventManager& events) const;

        void Draw(ResourceManager& resources, EventManager& events) const;

        std::string Label;
        std::vector<std::unique_ptr<Element>> Elements;
    };

    struct MenuBarElement : Element
    {
        void Register(ResourceManager& resources, EventManager& events) override;
        void Release(ResourceManager& resources, EventManager& events) override;

        void Draw(ResourceManager& resources, EventManager& events) override;

        std::vector<Menu> Menus;
    };

    struct CheckboxElement : Element
    {
        void Register(ResourceManager& resources, EventManager& events) override;

        void Draw(ResourceManager& resources, EventManager& events) override;

        std::string Label;
        std::string Event;
        std::string Default;

        bool Var = false;
    };
}
