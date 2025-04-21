#ifndef __OPENGLFRAMEBUFFER_H__
#define __OPENGLFRAMEBUFFER_H__

#pragma once

#include "../../FrameBuffer.h"
#include "../../Types.h"

class OpenGLFrameBuffer : public FrameBuffer {
   public:
    OpenGLFrameBuffer(float width, float height);
    ~OpenGLFrameBuffer();

    void Bind() const override;
    void Unbind() const override;

    void Clear() const override;
    void Resize(float width, float height) override;

    uint32 GetColorAttachmentTexture() const override;

   private:
    uint32 fbo;
    uint32 texture;
    uint32 rbo;

    void Invalidate(float width, float height);
};

#endif  // __OPENGLFRAMEBUFFER_H__