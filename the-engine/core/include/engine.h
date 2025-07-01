#pragma once


namespace TheEngine::Core

{
    class Engine // Or AssetManager
    {
    private:
        std::unique_ptr<TheEngine::Resource::ResourceManager> m_cpuResourceManager;
        std::unique_ptr<TheEngine::Graphics::GPUResourceManager> m_gpuResourceManager;

        // Potentially a global GPU memory allocator
        // std::unique_ptr<TheEngine::Graphics::GPUMemoryAllocator> m_gpuMemoryAllocator;

    public:
        void initialize()
        {
    
            // m_gpuMemoryAllocator = std::make_unique<GPUMemoryAllocator>(...);
            m_cpuResourceManager = std::make_unique<TheEngine::Resource::ResourceManager>();
            m_gpuResourceManager = std::make_unique<TheEngine::Graphics::GPUResourceManager>(/* m_renderDevice.get() */); // Needs render device

            // The Orchestrator needs access to both managers and the allocator
            m_gpuAssetOrchestrator = std::make_unique<TheEngine::Graphics::GPUAssetOrchestrator>(
                m_cpuResourceManager.get(),
                m_gpuResourceManager.get()
                // m_gpuMemoryAllocator.get() // if it's a separate dependency
            );
        }

        // Method to request asset loading/upload from the higher level
        void loadAsset(const TheEngine::Core::ResourceHandle handle)
        {
            // This is the primary entry point for requesting uploads
            m_gpuAssetOrchestrator->requestGPUUpload(handle);
        }
        // ...
    };
}