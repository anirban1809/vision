#ifndef __RENDERCONTEXT_H__
#define __RENDERCONTEXT_H__

#include "Core/Types.h"
#include "Core/VertexLayout.h"
#include <vector>
class RenderContext {
   public:
    virtual void Draw(const std::vector<float>& vertices,
                      const std::vector<uint32>& indices,
                      const VertexLayout& layout) = 0;
    virtual void Clear() = 0;
};
#endif  // __RENDERCONTEXT_H__