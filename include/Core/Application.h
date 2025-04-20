#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Document.h"
#include "Window.h"
#include <string>

class Application {
   public:
    Application(int, int, const std::string&);
    void LoadSource(const std::string&);
    void Run();

   public:
    void OnInit();
    void OnUpdate();
    void OnRender();

   private:
    std::string source;
    Window window;
    Document document;
};

#endif  // __APPLICATION_H__