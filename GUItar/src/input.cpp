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
    const auto [buttons_, axes_] = GetGamepad(jid, true);

    float accum = 0.0f;
    for (const auto& [type_, index_, negate_] : m_Axes[id])
    {
        float value;
        switch (type_)
        {
        case AxisType_Key:
            value = GetKey(index_) ? 1.0f : 0.0f;
            break;
        case AxisType_Button:
            value = index_ >= 0 && index_ < std::size(buttons_) && buttons_[index_] == GLFW_PRESS ? 1.0f : 0.0f;
            break;
        case AxisType_Axis:
            value = index_ >= 0 && index_ < std::size(axes_) ? axes_[index_] : 0.0f;
            break;
        default:
            continue;
        }

        accum += negate_ ? -value : value;
    }

    return accum;
}

void guitar::InputManager::Update(GLFWwindow* window)
{
    for (int i = 32; i < GLFW_KEY_LAST; ++i)
        if (const int state = glfwGetKey(window, i); state != GLFW_INVALID_ENUM)
            m_Keys[i].Update(state);
}
