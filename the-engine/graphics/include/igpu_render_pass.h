#pragma once
#include <cstdint>
#include <vector>
#include "igpu_texture.h"

// need more cleaning can skip for now
namespace TheEngine::Graphics
{


    // How an attachment's data is handled at the start of a render pass or subpass.
    enum class AttachmentLoadOp
    {
        LOAD,    // Preserve existing contents of the attachment.
        CLEAR,   // Clear the attachment to a specific clear value.
        DONT_CARE // Don't care about the contents; contents are undefined.
    };

    // How an attachment's data is handled at the end of a render pass or subpass.
    enum class AttachmentStoreOp
    {
        STORE,   // Store the rendered contents of the attachment.
        DONT_CARE // Don't care about the contents; contents are undefined.
    };



    // Defines the way an attachment is used within a subpass.
    enum class AttachmentReferenceType
    {
        UNDEFINED,
        INPUT,      // Read from as an input attachment (from a previous subpass).
        COLOR,      // Rendered into as a color attachment.
        RESOLVE,    // Used as a resolve target for a multisampled color attachment.
        DEPTH_STENCIL, // Rendered into as a depth/stencil attachment.
        PRESERVE    // Not used in this subpass, but its contents should be preserved.
    };

    // Defines the access pattern for memory dependencies between subpasses.
    // These correspond directly to VkAccessFlagBits in Vulkan.
    enum class AccessFlags : uint32_t
    {
        NONE = 0,
        SHADER_READ = 1 << 0,
        SHADER_WRITE = 1 << 1,
        COLOR_ATTACHMENT_READ = 1 << 2,
        COLOR_ATTACHMENT_WRITE = 1 << 3,
        DEPTH_STENCIL_ATTACHMENT_READ = 1 << 4,
        DEPTH_STENCIL_ATTACHMENT_WRITE = 1 << 5,
       
    };

    // Defines the pipeline stages involved in a dependency.
    // These correspond directly to VkPipelineStageFlagBits in Vulkan.
    enum class PipelineStageFlags : uint32_t
    {// clean later
        NONE = 0,
        TOP_OF_PIPE = 1 << 0,
        DRAW_INDIRECT = 1 << 1,
        VERTEX_INPUT = 1 << 2,
        VERTEX_SHADER = 1 << 3,
        TESSELLATION_CONTROL_SHADER = 1 << 4,
        TESSELLATIONEVALUATIONSHADER = 1 << 5,
        GEOMETRY_SHADER = 1 << 6,
        FRAGMENT_SHADER = 1 << 7,
        
        EarlyFragmentTests = 1 << 8,
        LateFragmentTests = 1 << 9,
        COLOR_ATTACHMENT_OUTPUT = 1 << 10,
        ComputeShader = 1 << 11,
        Transfer = 1 << 12,
        BottomOfPipe = 1 << 13,
        Host = 1 << 14,
        AllGraphics = 1 << 15,
        AllCommands = 1 << 16,
  
    };








    // Describes a single attachment used in the render pass.
    struct AttachmentDescription
    {
        TextureFormat format;


        AttachmentLoadOp loadOp;
        AttachmentStoreOp storeOp;



        TextureLayout initialLayout; // Layout at the start of the render pass
        TextureLayout finalLayout;   // Layout at the end of the render pass

    };

    // Reference to an attachment within a subpass.
    struct AttachmentReference
    {
        uint32_t attachmentIndex; // Index into the RenderPassInfo::attachments vector
        TextureLayout layout;     // Layout in which the attachment will be used in this subpass

    };

    // Describes a single subpass within the render pass.
    struct SubpassDescription
    {
        // For input attachments, the layout will be TextureLayout::ShaderReadOnlyOptimal
        std::vector<AttachmentReference> inputAttachments;

        // For color attachments, the layout will be TextureLayout::ColorAttachmentOptimal
        std::vector<AttachmentReference> colorAttachments;



        // For depth/stencil attachment, layout will be TextureLayout::DepthStencilAttachmentOptimal
        AttachmentReference depthStencilAttachment; // Only one depth/stencil attachment
        bool hasDepthStencil = false; // Indicates if depthStencilAttachment is valid

        // Attachments whose contents must be preserved but are not used as input/output
        std::vector<uint32_t> preserveAttachments; // Indices into the main attachments vector
    };

    // Defines a dependency between subpasses (or external operations).
    // Crucial for synchronization and layout transitions in Vulkan.
    struct SubpassDependency
    {
        uint32_t srcSubpass;    // Source subpass index (VK_SUBPASS_EXTERNAL for external operations)
        uint32_t dstSubpass;    // Destination subpass index (VK_SUBPASS_EXTERNAL for external operations)

        PipelineStageFlags srcStageMask;    // Pipeline stages in srcSubpass that must complete
        PipelineStageFlags dstStageMask;    // Pipeline stages in dstSubpass that must wait

        AccessFlags srcAccessMask;          // Memory access types in srcSubpass that must complete
        AccessFlags dstAccessMask;          // Memory access types in dstSubpass that must wait

        // Dependency flags (e.g., VK_DEPENDENCY_BY_REGION_BIT)
        // uint32_t dependencyFlags = 0;
    };

    // Information required to create an IGPURenderPass.
    struct RenderPassInfo
    {
        std::vector<AttachmentDescription> attachments;
        std::vector<SubpassDescription> subpasses;
        std::vector<SubpassDependency> dependencies;
    };







    // The IGPURenderPass interface represents a sequence of rendering operations
   // targeting a set of attachments.
   // This is explicitly used by pipelines (GraphicsPipelineState) and framebuffers
   // in Vulkan, and conceptually by the rendering commands themselves in OpenGL/DX12.
    class IGPURenderPass : public IGPUResource
    {

        

    public:
        // Virtual destructor for proper polymorphic cleanup.
        virtual ~IGPURenderPass() = default;
       // virtual size_t  GetAttachmentCount() const = 0 ;
        // Optionally, expose the info it was created with for debugging or validation
        // virtual const RenderPassInfo& GetRenderPassInfo() const = 0;
    };




}
