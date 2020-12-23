//
// Created by matt on 2020-07-12.
//

#include "CE_Input.h"

#include <UT/UT_Application.h>
#include <UT/UT_Window.h>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace dogb::CE::Input
{

bool
IsKeyPressed(KeyType key)
{
    auto window = reinterpret_cast<GLFWwindow*>(UT::AppInterface::mainApp()->mainWindow()->nativeWindow());
    int keycode = static_cast<int>(key);
    auto state = glfwGetKey(window, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool
IsButtonPressed(MouseButtonType button)
{
    auto window = reinterpret_cast<GLFWwindow*>(UT::AppInterface::mainApp()->mainWindow()->nativeWindow());
    int button_code = static_cast<int>(button);
    auto state = glfwGetMouseButton(window, button_code);
    return state == GLFW_PRESS;
}

float GetMouseX()
{
    auto window = reinterpret_cast<GLFWwindow*>(UT::AppInterface::mainApp()->mainWindow()->nativeWindow());
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    return static_cast<float>(x);
}

float GetMouseY()
{
    auto window = reinterpret_cast<GLFWwindow*>(UT::AppInterface::mainApp()->mainWindow()->nativeWindow());
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    return static_cast<float>(y);
}

std::pair<float, float> GetMousePos()
{
    auto window = reinterpret_cast<GLFWwindow*>(UT::AppInterface::mainApp()->mainWindow()->nativeWindow());
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    return std::make_pair(static_cast<float>(x), static_cast<float>(y));
}

}