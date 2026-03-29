#pragma once




namespace TheEngine::RenderingSystem
{


    //This Frame buffer contains actual data to be attached at pso inputs
    struct FramebufferAttachment 
    {

        TextureHandle texture; // Agnostic handle 
        uint32_t mipLevel = 0;
        uint32_t layer = 0;

        // For Vulkan: Clear values
        float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
        float clearDepth = 1.0f;
        uint32_t clearStencil = 0;
    };





    //This class will be given to api specific IFramebuffer class which will either create rendercreate info and attach to cmd or in opengl attach it to actual framebuffer
    // In opengl we need to have a fbo handle for it
    //concrete class
    class Framebuffer
    {


    protected:

        uint32_t m_width;
        uint32_t m_height;

        std::vector<FramebufferAttachment> m_colorAttachments;
        FramebufferAttachment m_depthStencilAttachment;

        bool m_hasDepthStencil = false;

    public:

        Framebuffer(uint32_t width, uint32_t height)
            : m_width(width), m_height(height) 
        {

        }

        ~Framebuffer() = default;

        // The "Swap" mechanism
        void setColorAttachment(uint32_t index, const FramebufferAttachment& attachment) = 0;
        void setDepthStencilAttachment(const FramebufferAttachment& attachment) = 0;


        uint32_t getWidth() const { return m_width; }
        uint32_t getHeight() const { return m_height; }


    };



}