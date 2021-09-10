#include "hpch.h"
#include "Core/Input.h"

#include "Core/Application.h"
#include <GLFW/glfw3.h>

namespace HEngine {

bool Input::IsKeyPressed(const KeyCode code)
{
    auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    auto state = glfwGetKey(window, code);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(const MouseCode code)
{
    auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    auto state = glfwGetMouseButton(window, code);
    return state == GLFW_PRESS;
}

std::pair<float, float> Input::GetMousePosition()
{
    auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    double xpos = 0, ypos = 0;
    glfwGetCursorPos(window, &xpos, &ypos);

    return { (float)xpos, (float)ypos };
}

float Input::GetMouseX()
{
    auto [x, y] = GetMousePosition();
    return x;
}

float Input::GetMouseY()
{
    auto [x, y] = GetMousePosition();
    return y;
}
}
