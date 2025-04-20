#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <GLFW/glfw3.h>

class Window {
   public:
    GLFWwindow *GetGLFWWindow();

   private:
    GLFWwindow *glfwWindow;
};
#endif  // __WINDOW_H__