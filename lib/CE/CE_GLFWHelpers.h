//
// Created by matt on 2020-07-11.
//

#ifndef DOGBONE_CE_GLFWHELPERS_H
#define DOGBONE_CE_GLFWHELPERS_H

#include "CE_API.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <memory>

namespace dogb::CE
{
struct CE_API glfw_deleter
{
    void operator()(GLFWwindow* ptr)
    {
        glfwDestroyWindow(ptr);
    }
};

using WindowDeleter = glfw_deleter;
using WindowType = GLFWwindow;
using WindowPtr = std::unique_ptr<WindowType, WindowDeleter>;
}

#endif // DOGBONE_CE_GLFWHELPERS_H
