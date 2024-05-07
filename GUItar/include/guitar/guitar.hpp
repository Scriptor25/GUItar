#pragma once

#include <string>

namespace guitar
{
    class Application;
    class ResourceManager;
    class EventManager;

    struct Layout;
    struct Element;
    struct DockSpaceElement;
    struct DemoElement;
    struct WindowElement;
    struct ButtonElement;
    struct ImageElement;
    struct TextElement;
    struct SimpleElement;
    struct Image;

    struct AppConfig
    {
        std::string Title = "Application";
        std::string Version = "1.0.0";
        std::string Layout = "main";
        int Width = 0;
        int Height = 0;
    };
}
