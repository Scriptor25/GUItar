#pragma once

#include <map>
#include <string>
#include <unordered_map>
#include <vector>

namespace guitar
{
    struct Joystick
    {
        void Dump();

        std::string Name;

        std::vector<float> Axes;
        std::vector<unsigned char> Buttons;
        std::vector<unsigned char> Hats;
    };

    struct KeyState
    {
        void Next(bool state);

        bool Now;
        bool Pre;
    };

    class InputManager
    {
    public:
        static Joystick GetJoystick(int index);
        static std::map<int, std::string> ListJoysticks();

        bool GetKey(int key);
        bool GetKeyPress(int key);
        bool GetKeyRelease(int key);
        bool GetKeyRepeat(int key);

        void Update(GLFWwindow* pWindow);

    private:
        std::unordered_map<int, KeyState> m_Keys;
    };
}
