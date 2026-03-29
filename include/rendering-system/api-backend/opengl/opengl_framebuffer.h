#pragma once


namespace TheEngine::RenderingSystem::OpenGLBackend
{
    //In New desing we dont use this, but we need the code for what is being done here moved to somewhere else

    class OpenglFramebuffer : public IFramebuffer 
    {
    private:

        uint32_t m_fboHandle = 0;//change to GLuint

    public:

        OpenglFramebuffer(uint32_t width, uint32_t height, GLuint fboHandle);

        virtual ~OpenglFramebuffer() override;
    

        void setColorAttachment(uint32_t index, const FramebufferAttachment& attachment) override; 


        void setDepthStencilAttachment(const FramebufferAttachment& attachment) override;
    
        //api specific method
        uint32_t getNativeHandle() const;



    };


}