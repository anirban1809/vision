#ifndef __INDEXBUFFER_H__
#define __INDEXBUFFER_H__

#include "Types.h"
#include <memory>
class IndexBuffer {
   public:
    virtual ~IndexBuffer() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual uint32 GetCount() const = 0;
    static std::shared_ptr<IndexBuffer> Create(const uint32*, uint32);
};
#endif  // __INDEXBUFFER_H__