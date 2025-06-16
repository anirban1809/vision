#include "../CocoaHelper.h"
#include "Core/Window.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <string>

Window::Window(int width, int height, const std::string& name) {
    // --------- Initialize GLFW ----------
    if (!glfwInit()) {
        std::cerr << "Could not init GLFW\n";
        return;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    int win_width = 1024, win_height = 768;
    window = glfwCreateWindow(win_width, win_height,
                              "OpenGL TTF Font Rendering", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    MakeWindowBorderless(window);
}

bool Window::ShouldClose() const { return glfwWindowShouldClose(window); }

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}