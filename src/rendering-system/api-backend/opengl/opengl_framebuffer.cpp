


namespace TheEngine::RenderingSystem::OpenGLBackend
{



    OpenglFramebuffer::OpenglFramebuffer(uint32_t width, uint32_t height) : 
        IFramebuffer(width, height)
        {
            glCreateFramebuffers(1, &m_fboHandle);
        }



    virtual OpenglFramebuffer::~OpenglFrameBuffer() override
        {
            if (m_fboHandle) glDeleteFramebuffers(1, &m_fboHandle);
        }


    virtual void OpenglFramebuffer::setColorAttachment(uint32_t index, const FramebufferAttachment& attachment) override
    {
            // 1. Update internal tracking
            if (index >= m_colorAttachments.size()) m_colorAttachments.resize(index + 1);
            m_colorAttachments[index] = attachment;

            // 2. Modern OpenGL Swap (DSA)
            glNamedFramebufferTexture(m_fboHandle, GL_COLOR_ATTACHMENT0 + index,
                (GLuint)attachment.texture.apiHandle, attachment.mipLevel);

            // 3. Update Draw Buffers routing (The "Pipes")
            std::vector<GLenum> drawBuffers;
            for (int i = 0; i < m_colorAttachments.size(); ++i) {
                drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);
            }
            glNamedFramebufferDrawBuffers(m_fboHandle, (GLsizei)drawBuffers.size(), drawBuffers.data());
        }

    virtual void OpenglFramebuffer::setDepthStencilAttachment(const FramebufferAttachment& attachment) override 
    {
            m_depthStencilAttachment = attachment;
            m_hasDepthStencil = true;
            glNamedFramebufferTexture(m_fboHandle, GL_DEPTH_STENCIL_ATTACHMENT,
                (GLuint)attachment.texture.apiHandle, attachment.mipLevel);
    }




    virtual uint32_t OpenglFramebuffer::getNativeHandle() const override
    { 
        return m_fboHandle; 
    }



}