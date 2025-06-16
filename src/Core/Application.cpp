#include "Core/Application.h"
#include <string>

Application::Application(int width, int height, const std::string& title) {
    window = new Window(width, height, title);
}

void Application::Run() {
    OnInit();
    while (!window->ShouldClose()) {
        OnUpdate();
        OnRender();
    }
}