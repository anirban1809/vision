#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include "Types.h"
#pragma once

#include "../Core/Types.h"
#include <memory>

/**
 * @class FrameBuffer
 * @brief Abstract base class for managing framebuffers in a rendering pipeline.
 *
 * The FrameBuffer class provides an interface for creating and managing
 * framebuffers, which are used to render graphics off-screen or to textures.
 * Derived classes must implement the pure virtual methods to provide specific
 * functionality.
 *
 * Key Features:
 * - Bind and unbind the framebuffer for rendering.
 * - Clear the framebuffer contents.
 * - Resize the framebuffer to specified dimensions.
 * - Retrieve the texture ID of the color attachment.
 * - Factory method for creating platform-specific framebuffer instances.
 */
class FrameBuffer {
   public:
    /**
     * @brief Virtual destructor for the FrameBuffer class.
     *
     * Ensures proper cleanup of resources in derived classes when a FrameBuffer
     * object is destroyed. Declared as default to use the compiler-generated
     * implementation.
     */
    virtual ~FrameBuffer() = default;

    /**
     * @brief Binds the framebuffer for rendering or other operations.
     *
     * This method is responsible for making the framebuffer active in the
     * rendering pipeline. It must be implemented by derived classes to
     * perform the necessary binding operations specific to the framebuffer
     * implementation.
     *
     * @note This is a pure virtual function and must be overridden in
     *       derived classes.
     */
    virtual void Bind() const = 0;

    /**
     * @brief Unbinds the framebuffer from the current rendering context.
     *
     * This function is used to detach the framebuffer, restoring the default
     * framebuffer or the previously bound framebuffer. It is typically called
     * after rendering operations are complete.
     */
    virtual void Unbind() const = 0;

    /**
     * @brief Clears the framebuffer, resetting its contents.
     *
     * This is a pure virtual function that must be implemented by derived
     * classes. It is typically used to clear the framebuffer to a default
     * state, such as setting all pixels to a specific color or clearing
     * depth/stencil buffers.
     */
    virtual void Clear() const = 0;

    /**
     * @brief Resizes the framebuffer to the specified width and height.
     *
     * This function adjusts the dimensions of the framebuffer, allowing it
     * to accommodate new rendering sizes. Implementations of this method
     * should handle any necessary resource reallocation or updates to
     * ensure proper functionality after resizing.
     *
     * @param width The new width of the framebuffer in pixels.
     * @param height The new height of the framebuffer in pixels.
     */
    virtual void Resize(float width, float height) = 0;

    /**
     * @brief Retrieves the texture ID of the color attachment.
     *
     * This function returns the identifier of the texture associated with the
     * color attachment of the framebuffer. The texture ID can be used for
     * operations such as binding the texture or performing rendering tasks.
     *
     * @return uint32 The texture ID of the color attachment.
     */
    virtual uint32 GetColorAttachmentTexture() const = 0;

    /**
     * @brief Creates a new FrameBuffer instance with the specified width and
     * height.
     *
     * @param width The width of the framebuffer.
     * @param height The height of the framebuffer.
     * @return A shared pointer to the newly created FrameBuffer instance.
     */
    static std::shared_ptr<FrameBuffer> Create(float width, float height);
};

#endif  // __FRAMEBUFFER_H__