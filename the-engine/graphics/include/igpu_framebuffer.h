#pragma once
#include "igpu_texture_view.h"
#include "igpu_render_pass.h"
#include "igpu_resource.h"
//cleaned
namespace TheEngine::Graphics
{
    enum class TextureLayout
    {
        UNDEFINED,                  // Initial state, content undefined
        GENERAL,                    // General purpose layout

        COLOR_ATTACHMENT_OPTIMAL,     // Optimal for color attachments
        DEPTH_STENCIL_ATTACHMENT_OPTIMAL, // Optimal for depth/stencil attachments
        DEPTH_STENCIL_READONLY_OPTIMAL,   // Optimal for read-only depth/stencil

        SHADER_READONLY_OPTIMAL,      // Optimal for sampling in shaders

        TRANSFER_SRC_OPTIMAL,         // Optimal for source of transfer operations
        TRANSFER_DST_OPTIMAL,         // Optimal for destination of transfer operations

        PRESENT_SRC,                 // Optimal for presenting to screen (swapchain image)
        SHARED_PRESENT              // Shared layout for presentation
        // ... potentially more specific layouts based on usage
    };


	enum class Filter
	{
		NEAREST,   // Nearest neighbor filtering
		LINEAR,    // Linear filtering
		CUBIC,     // Bicubic filtering (if supported)
		// ... other filters as needed
	};








    // Defines the semantic type of an attachment within a framebuffer.
    enum class FramebufferAttachmentType : uint32_t
    {
        COLOR_0 = 0,
        COLOR_1,
        COLOR_2,
        COLOR_3,
        COLOR_4,
        COLOR_5,
        COLOR_6,
        COLOR_7, // Up to 8 color attachments generally supported

        DEPTH,
        STENCIL,
        DEPTH_STENCIL, // Combined depth and stencil attachment

        // Add more if needed, e.g., Resolve attachments
    };


    // Represents a single attachment within the framebuffer.
    struct FramebufferAttachment
    {
        FramebufferAttachmentType type; // Semantic type (e.g., Color0, Depth)
        IGPUTextureView* pView;         // The actual texture view to attach

    };

    // Information required to create an IGPUFramebuffer.
    struct FramebufferCreateInfo
    {
        // The render pass this framebuffer will be compatible with.
        // In Vulkan, VkFramebuffer creation explicitly references a VkRenderPass.
        // In OpenGL/DX12, this helps ensure the attachment formats match the
        // render pass's implicit expectations or shader requirements.
        IGPURenderPass* pRenderPass = nullptr;

        // The list of texture views that will serve as the attachments.
        // The order here should typically match the attachment order in the RenderPassInfo
        // used to create pRenderPass.
        std::vector<FramebufferAttachment> attachments;

        // Dimensions of the framebuffer. All attached texture views should be
        // compatible with these dimensions.
        uint32_t width = 0;
        uint32_t height = 0;

        // All attachments should have at least this many layers.
    };



    // The IGPUFramebuffer interface represents a collection of image attachments
       // that graphics commands render into.
       // This is created via the IGPURenderDevice.
    class IGPUFramebuffer : public IGPUResource
    {
    protected:
        // Protected constructor to prevent direct instantiation.
        // Framebuffers are created via the IGPURenderDevice.
        IGPUFramebuffer() = default;

    public:

        // Virtual destructor for proper polymorphic cleanup.
        virtual ~IGPUFramebuffer() = default;

        // --- Query methods ---
        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;



        // Optionally, get all attachments (less common for public API)
         virtual const std::vector<FramebufferAttachment>& GetAttachments() const = 0;
    };
}