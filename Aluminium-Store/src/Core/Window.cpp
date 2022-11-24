#include "Window.h"

#include "Application.h"

#include <GLFW/glfw3.h>

#define window (GLFWwindow*) wPointer

namespace Aluminium {

	void Window::Initialize(uint32_t width, uint32_t height, std::string title) {

		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		wPointer = glfwCreateWindow(1280, 720, "Aluminium Store", nullptr, nullptr);
		glfwMakeContextCurrent(window);
		glfwMaximizeWindow(window);

		glfwSetWindowCloseCallback(window, [](GLFWwindow* win) {

			MainApplication::Shutdown();

		});

	}

	void Window::Update() {

		glfwPollEvents();
		glfwSwapBuffers(window);

	}

	void Window::Shutdown() {

		glfwDestroyWindow(window);
		glfwTerminate();

	}

}