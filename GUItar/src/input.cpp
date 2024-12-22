#define GLFW_INCLUDE_NONE

#include <iostream>
#include <GLFW/glfw3.h>
#include <guitar/input.hpp>

void guitar::KeyState::Update(const bool state)
{
    Pre = Now;
    Now = state;
}

GLFWgamepadstate guitar::InputManager::GetGamepad(const int jid, const bool silent)
{
    if (jid < GLFW_JOYSTICK_1 || jid > GLFW_JOYSTICK_16)
    {
        if (!silent) std::cerr << "Joystick index out of range [0;15]: " << jid << std::endl;
        return {};
    }

    if (!glfwJoystickPresent(jid))
    {
        if (!silent) std::cerr << "Joystick " << jid << " is not present" << std::endl;
        return {};
    }

    if (!glfwJoystickIsGamepad(jid))
    {
        const auto name = glfwGetJoystickName(jid);
        const auto guid = glfwGetJoystickGUID(jid);
        if (!silent)
            std::cerr
                << "Joystick "
                << jid
                << " (name=" << name
                << ", guid=" << guid
                << ") is not a gamepad" << std::endl;
        return {};
    }

    GLFWgamepadstate state;
    glfwGetGamepadState(jid, &state);

    return state;
}

std::map<int, std::string> guitar::InputManager::ListJoysticks()
{
    std::map<int, std::string> joysticks;

    for (int i = GLFW_JOYSTICK_1; i <= GLFW_JOYSTICK_16; ++i)
        if (glfwJoystickPresent(i))
            joysticks[i] = glfwGetJoystickName(i);

    return joysticks;
}

bool guitar::InputManager::GetKey(const int key)
{
    return m_Keys[key].Now;
}

bool guitar::InputManager::GetKeyPress(const int key)
{
    return !m_Keys[key].Pre && m_Keys[key].Now;
}

bool guitar::InputManager::GetKeyRelease(const int key)
{
    return m_Keys[key].Pre && !m_Keys[key].Now;
}

bool guitar::InputManager::GetKeyRepeat(const int key)
{
    return m_Keys[key].Pre && m_Keys[key].Now;
}

void guitar::InputManager::CreateAxis(const std::string& id, const std::vector<AxisConfig>& config)
{
    m_Axes[id] = config;
}

float guitar::InputManager::GetAxis(const int jid, const std::string& id)
{
    const auto [Buttons, Axes] = GetGamepad(jid, true);

    float accum = 0.0f;
    for (const auto& [Type, Index, Negate] : m_Axes[id])
    {
        float value;
        switch (Type)
        {
        case AxisType_Key:
            value = GetKey(Index) ? 1.0f : 0.0f;
            break;
        case AxisType_Button:
            value = Index >= 0 && Index < std::size(Buttons) && Buttons[Index] == GLFW_PRESS ? 1.0f : 0.0f;
            break;
        case AxisType_Axis:
            value = Index >= 0 && Index < std::size(Axes) ? Axes[Index] : 0.0f;
            break;
        default:
            continue;
        }

        accum += Negate ? -value : value;
    }

    return accum;
}

void guitar::InputManager::Update(GLFWwindow* pWindow)
{
    for (int i = 32; i < GLFW_KEY_LAST; ++i)
        if (const int state = glfwGetKey(pWindow, i); state != GLFW_INVALID_ENUM)
            m_Keys[i].Update(state);
}
