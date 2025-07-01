#pragma once

//cleaned
namespace TheEngine::Graphics
{

    // Example TextureViewCreateInfo
    struct TextureViewCreateInfo
    {
        IGPUTexture* pTexture; // The texture object this view is for
  
        TextureFormat format; // Can be different from pTexture->GetFormat() for typeless images

       
 
    };


    //data interpretation
    class IGPUTextureView : public IGPUResource
    {
    protected:
        IGPUTextureView() = default;
    public:
        virtual ~IGPUTextureView() = default;

   
        

        // Get the texture this view refers to (not owned)
        virtual IGPUTexture* GetTexture() const = 0;
        virtual TextureFormat GetFormat() const = 0; // The format as seen by the view




    
    };

}