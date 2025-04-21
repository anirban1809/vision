#ifndef __VERTEXLAYOUT_H__
#define __VERTEXLAYOUT_H__

#include "Types.h"
#include <vector>

enum class ShaderType {
    Float,
    Float2,
    Float3,
    Float4,
    Mat3,
    Mat4,
    Int,
    Int2,
    Int3,
    Int4,
    Bool
};

uint32 GetShaderDataTypeSize(ShaderType type);

struct VertexAttribute {
    ShaderType type;
    uint32 offset;
    uint32 location;

    VertexAttribute(ShaderType t, uint32 o, uint32 loc)
        : type(t), offset(o), location(loc) {}
};

class VertexLayout {
   public:
    VertexLayout() = default;

    VertexLayout(std::initializer_list<VertexAttribute> attributes)
        : attributes(attributes) {
        CalculateStride();
    }

    uint32 GetStride() const { return stride; }
    const std::vector<VertexAttribute>& GetAttributes() const {
        return attributes;
    }

   private:
    void CalculateStride();

   private:
    std::vector<VertexAttribute> attributes;
    uint32 stride = 0;
};
#endif  // __VERTEXLAYOUT_H__