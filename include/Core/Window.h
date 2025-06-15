#include <string>
#include <GLFW/glfw3.h>
class Window {
   public:
    Window(int width, int height, const std::string& name);
    ~Window();
    bool ShouldClose() const;

   private:
    GLFWwindow* window;
};