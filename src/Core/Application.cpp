#include <GL/glew.h>
#include "Core/Application.h"
#include <iostream>
#include <string>

Application::Application(int width, int height, const std::string& title) {
    window = new Window(width, height, title);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to init GLEW\n";
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Application::Run() {
    OnInit();
    while (!window->ShouldClose()) {
        OnUpdate();
        OnRender();
    }
}