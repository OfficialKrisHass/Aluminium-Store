#include "Window.h"

#include <GLFW/glfw3.h>

#include <iostream>

namespace Aluminium::Window {

    GLFWwindow* window = nullptr;

    void Initialize() {

        int init = glfwInit();
        AL_ASSERT(init, "Failed to initialize GLFW");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(1280, 720, "Aluminium", nullptr, nullptr);
        AL_ASSERT(window, "Failed to create window");

        glfwMakeContextCurrent(window);

        std::cout << "Window opened";

    }
    void Update() {

        glfwPollEvents();
        glfwSwapBuffers(window);

    }
    void Shutdown() {

        glfwDestroyWindow(window);
        glfwTerminate();

    }

    bool IsWindowOpen() { return !glfwWindowShouldClose(window); }

    void* GetPointer() { return window; }

}
