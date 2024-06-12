#include <GLFW/glfw3.h>
#include <guitar/input.hpp>

void guitar::KeyState::Next(const bool state)
{
    Pre = Now;
    Now = state;
}

guitar::Joystick guitar::InputManager::GetJoystick(const int index)
{
    if (index < GLFW_JOYSTICK_1 || index > GLFW_JOYSTICK_16 || !glfwJoystickPresent(index))
        return {};

    Joystick joystick;
    joystick.Name = glfwGetJoystickName(index);

    int axis_count, button_count, hat_count;
    const auto axes = glfwGetJoystickAxes(index, &axis_count);
    const auto buttons = glfwGetJoystickButtons(index, &button_count);
    const auto hats = glfwGetJoystickHats(index, &hat_count);

    joystick.Axes.resize(axis_count);
    for (size_t i = 0; i < axis_count; ++i)
        joystick.Axes[i] = axes[i];

    joystick.Buttons.resize(button_count);
    for (size_t i = 0; i < button_count; ++i)
        joystick.Buttons[i] = buttons[i];

    joystick.Hats.resize(hat_count);
    for (size_t i = 0; i < hat_count; ++i)
        joystick.Hats[i] = hats[i];

    return joystick;
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

void guitar::InputManager::Update(GLFWwindow* pWindow)
{
    for (int i = 32; i < GLFW_KEY_LAST; ++i)
        if (const int state = glfwGetKey(pWindow, i); state != GLFW_INVALID_ENUM)
            m_Keys[i].Next(state);
}
