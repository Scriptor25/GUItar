#pragma once

#include <string>

namespace guitar
{
    class Application;
    class ResourceManager;
    class EventManager;

    struct Layout;
    struct Element;
    struct Menu;

    struct Image;

    struct AppConfig
    {
        std::string Title = "Application";
        std::string Version = "1.0.0";
        std::string Layout = "main";
        std::string IniDirectory = "ini";

        int Width = 0;
        int Height = 0;

        bool Viewports = true;
    };
}
