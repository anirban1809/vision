#pragma once

#include <string>
#include "Core/Window.h"
class Application {
   public:
    Application(int width, int height, const std::string& title);
    ~Application();

    void Run();  // Main loop
   protected:
    virtual void OnInit() {};    // To be overridden for custom initialization
    virtual void OnUpdate() {};  // Override for updating logic
    virtual void OnRender() {};  // Override for custom rendering
    Window* window;
};