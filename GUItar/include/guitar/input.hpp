#pragma once

#include <map>
#include <string>
#include <vector>

namespace guitar
{
    struct KeyState
    {
        void Update(bool state);

        bool Now;
        bool Pre;
    };

    enum AxisType
    {
        AxisType_Key,
        AxisType_Button,
        AxisType_Axis,
    };

    struct AxisConfig
    {
        AxisType Type;
        int Index;
        bool Negate;
    };

    class InputManager
    {
    public:
        static GLFWgamepadstate GetGamepad(int jid, bool silent = false);
        static std::map<int, std::string> ListJoysticks();

        bool GetKey(int key);
        bool GetKeyPress(int key);
        bool GetKeyRelease(int key);
        bool GetKeyRepeat(int key);

        void CreateAxis(const std::string& id, const std::vector<AxisConfig>& config);
        float GetAxis(int jid, const std::string& id);

        void Update(GLFWwindow* pWindow);

    private:
        std::map<int, KeyState> m_Keys;

        std::map<std::string, std::vector<AxisConfig>> m_Axes;
    };
}
