#ifndef __OPENGLRENDERCONTEXT_H__
#define __OPENGLRENDERCONTEXT_H__

#include "Core/RenderContext.h"
#include "Core/VertexArray.h"

class OpenGLRenderContext : public RenderContext {
   public:
    void Draw(const std::vector<float>& vertices,
              const std::vector<uint32>& indices,
              const VertexLayout& layout) override;

    void Clear() override;

   private:
    std::vector<std::shared_ptr<VertexArray>> drawQueue;
};
#endif  // __OPENGLRENDERCONTEXT_H__