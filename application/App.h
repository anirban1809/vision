#pragma once

#include "Core/Application.h"
#include <string>
class Example : public Application {
   public:
    Example(int width, int height, const std::string& name);
    virtual void OnInit() override;
    virtual void OnRender() override;
    virtual void OnUpdate() override;
};