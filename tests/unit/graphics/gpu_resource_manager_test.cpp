// gpu_resource_manager_test.cpp
#include "gtest/gtest.h"


// --- Mock/Dummy Headers for Dependencies ---



namespace TheEngine::Core
{
  
    using ResourceHandle = unsigned int;
}

namespace TheEngine::Graphics
{
    // Base interface for all GPU resources
    class IGPUResource
    {
    public:
        enum class GPUResourceType { UNDEFINED, TEXTURE, BUFFER, MESH, MATERIAL };
        virtual ~IGPUResource() = default;
        virtual GPUResourceType getType() const = 0;
    };

    // Derived interface for GPU Textures
    class IGPUTexture : public IGPUResource
    {
    public:
        GPUResourceType getType() const override { return GPUResourceType::TEXTURE; }
        virtual void bind() = 0; ///dummy method
    };

    // Derived interface for GPU Buffers
    class IGPUBuffer : public IGPUResource
    {
    public:
        GPUResourceType getType() const override { return GPUResourceType::BUFFER; }
        virtual void create() = 0; 
  
    };

    // Mock Render Device
    class IGPURenderDevice
    {
    public:
        virtual ~IGPURenderDevice() = default;

    };

    // Mock GPU Resource Streamer
    class GPUResourceStreamer
    {
    public:
		GPUResourceStreamer() = default;
         ~GPUResourceStreamer() = default;
        
    };
}

// --- Using directives for convenience within the test file ---
using namespace TheEngine::Graphics;
using namespace TheEngine::Core;
#include "gpu_resource_manager.h" 
// --- Custom Test GPUResource Implementation ---
// This is a simple concrete class derived from IGPUResource for testing purposes.

class TestGPUResource : public IGPUResource
{
public:
    int id;
    // Static counter to track how many TestGPUResource objects are currently alive
    static int s_instanceCount;
    // Flag to indicate if this specific instance was destroyed
    bool m_wasDestroyed = false;

    GPUResourceType getType() const override
    {
        return GPUResourceType::UNDEFINED; // Return a default type for testing
    }

    TestGPUResource(int _id) : id(_id)
    {
        s_instanceCount++;
        // std::cout << "TestGPUResource " << id << " constructed. Count: " << s_instanceCount << std::endl;
    }

    ~TestGPUResource() override
    {
        s_instanceCount--;
        m_wasDestroyed = true;
        // std::cout << "TestGPUResource " << id << " destroyed. Count: " << s_instanceCount << std::endl;
    }

    // Prevent copy construction and assignment for unique_ptr compatibility
    TestGPUResource(const TestGPUResource&) = delete;
    TestGPUResource& operator=(const TestGPUResource&) = delete;
};

// Initialize the static member
int TestGPUResource::s_instanceCount = 0;

// --- Another Test GPUResource Type for Type Mismatch Tests ---
class AnotherTestGPUResource : public IGPUResource
{
public:
    int value;
    AnotherTestGPUResource(int val) : value(val) {}

    GPUResourceType getType() const override
    {
        return GPUResourceType::UNDEFINED; // Return a default type for testing
    }
};

// --- Test GPUTexture Implementation ---
class TestGPUTexture : public IGPUTexture
{
public:
    int textureId;
    TestGPUTexture(int id) : textureId(id) {}
    void bind() override {} // Dummy implementation
};

// --- Test GPUBuffer Implementation ---
class TestGPUBuffer : public IGPUBuffer
{
public:
    int bufferId;
    TestGPUBuffer(int id) : bufferId(id) {}
    void create() override {} // Dummy implementation
};


// --- GPUResourceManager Test Suite ---

TEST(GPUResourceManagerTest, ConstructorInitializesEmpty)
{
    MockGPURenderDevice renderDevice;
    std::unique_ptr<GPUResourceStreamer> streamer = std::make_unique<GPUResourceStreamer>();
    GPUResourceManager mgr(renderDevice, std::move(streamer));

    // Verify that the internal resource map is empty
    EXPECT_EQ(mgr.getResource<TestGPUResource>(0), nullptr); // Attempting to get anything should fail
}

TEST(GPUResourceManagerTest, StoreResource_FirstResource)
{
    MockGPURenderDevice renderDevice;
    std::unique_ptr<GPUResourceStreamer> streamer = std::make_unique<GPUResourceStreamer>();
    GPUResourceManager mgr(renderDevice, std::move(streamer));

    ResourceHandle handle1 = 100;
    std::unique_ptr<TestGPUResource> res1 = std::make_unique<TestGPUResource>(101);
    TestGPUResource* rawRes1 = res1.get(); // Keep raw pointer to verify content later

    mgr.storeResource(handle1, std::move(res1));

    // Retrieve and verify the resource
    TestGPUResource* retrievedRes = mgr.getResource<TestGPUResource>(handle1);
    ASSERT_NE(retrievedRes, nullptr); // Should not be null
    EXPECT_EQ(retrievedRes->id, 101); // Check content
    EXPECT_EQ(retrievedRes, rawRes1); // Ensure it's the same object
}

TEST(GPUResourceManagerTest, StoreResource_MultipleSequential)
{
    MockGPURenderDevice renderDevice;
    std::unique_ptr<GPUResourceStreamer> streamer = std::make_unique<GPUResourceStreamer>();
    GPUResourceManager mgr(renderDevice, std::move(streamer));

    ResourceHandle h0 = 0;
    ResourceHandle h1 = 1;
    ResourceHandle h2 = 2;

    mgr.storeResource(h0, std::make_unique<TestGPUResource>(0));
    mgr.storeResource(h1, std::make_unique<TestGPUResource>(1));
    mgr.storeResource(h2, std::make_unique<TestGPUResource>(2));

    EXPECT_NE(mgr.getResource<TestGPUResource>(h0), nullptr);
    EXPECT_NE(mgr.getResource<TestGPUResource>(h1), nullptr);
    EXPECT_NE(mgr.getResource<TestGPUResource>(h2), nullptr);

    EXPECT_EQ(mgr.getResource<TestGPUResource>(h1)->id, 1);
}

TEST(GPUResourceManagerTest, StoreResource_DuplicateHandleDoesNotOverwrite)
{
    MockGPURenderDevice renderDevice;
    std::unique_ptr<GPUResourceStreamer> streamer = std::make_unique<GPUResourceStreamer>();
    GPUResourceManager mgr(renderDevice, std::move(streamer));

    ResourceHandle handle = 50;
    std::unique_ptr<TestGPUResource> originalRes = std::make_unique<TestGPUResource>(100);
    TestGPUResource* rawOriginalRes = originalRes.get();
    mgr.storeResource(handle, std::move(originalRes));

    // Try to store another resource with the same handle
    std::unique_ptr<TestGPUResource> newRes = std::make_unique<TestGPUResource>(200);
    mgr.storeResource(handle, std::move(newRes)); // This call should be ignored by implementation

    // Verify that the original resource is still there
    TestGPUResource* retrieved = mgr.getResource<TestGPUResource>(handle);
    ASSERT_NE(retrieved, nullptr);
    EXPECT_EQ(retrieved->id, 100); // Should still be the original resource
    EXPECT_EQ(retrieved, rawOriginalRes);
}

TEST(GPUResourceManagerTest, GetResource_ValidHandleCorrectType)
{
    MockGPURenderDevice renderDevice;
    std::unique_ptr<GPUResourceStreamer> streamer = std::make_unique<GPUResourceStreamer>();
    GPUResourceManager mgr(renderDevice, std::move(streamer));

    ResourceHandle handle = 77;
    std::unique_ptr<TestGPUResource> res = std::make_unique<TestGPUResource>(55);
    mgr.storeResource(handle, std::move(res));

    TestGPUResource* retrieved = mgr.getResource<TestGPUResource>(handle);
    ASSERT_NE(retrieved, nullptr);
    EXPECT_EQ(retrieved->id, 55);
}

TEST(GPUResourceManagerTest, GetResource_ValidHandleSpecificDerivedType)
{
    MockGPURenderDevice renderDevice;
    std::unique_ptr<GPUResourceStreamer> streamer = std::make_unique<GPUResourceStreamer>();
    GPUResourceManager mgr(renderDevice, std::move(streamer));

    ResourceHandle textureHandle = 1;
    ResourceHandle bufferHandle = 2;

    mgr.storeResource(textureHandle, std::make_unique<TestGPUTexture>(101));
    mgr.storeResource(bufferHandle, std::make_unique<TestGPUBuffer>(202));

    TestGPUTexture* retrievedTexture = mgr.getResource<TestGPUTexture>(textureHandle);
    ASSERT_NE(retrievedTexture, nullptr);
    EXPECT_EQ(retrievedTexture->textureId, 101);
    EXPECT_EQ(retrievedTexture->getType(), IGPUResource::GPUResourceType::TEXTURE);

    TestGPUBuffer* retrievedBuffer = mgr.getResource<TestGPUBuffer>(bufferHandle);
    ASSERT_NE(retrievedBuffer, nullptr);
    EXPECT_EQ(retrievedBuffer->bufferId, 202);
    EXPECT_EQ(retrievedBuffer->getType(), IGPUResource::GPUResourceType::BUFFER);
}


TEST(GPUResourceManagerTest, GetResource_InvalidHandle)
{
    MockGPURenderDevice renderDevice;
    std::unique_ptr<GPUResourceStreamer> streamer = std::make_unique<GPUResourceStreamer>();
    GPUResourceManager mgr(renderDevice, std::move(streamer));

    mgr.storeResource(10, std::make_unique<TestGPUResource>(1)); // Add one resource

    // Test non-existent handles
    EXPECT_EQ(mgr.getResource<TestGPUResource>(999), nullptr); // Non-existent handle
    EXPECT_EQ(mgr.getResource<TestGPUResource>(11), nullptr);  // Another non-existent handle
}

TEST(GPUResourceManagerTest, GetResource_RemovedResource)
{
    MockGPURenderDevice renderDevice;
    std::unique_ptr<GPUResourceStreamer> streamer = std::make_unique<GPUResourceStreamer>();
    GPUResourceManager mgr(renderDevice, std::move(streamer));

    ResourceHandle handle = 20;
    mgr.storeResource(handle, std::make_unique<TestGPUResource>(1));
    mgr.removeResource(handle); // Remove it

    EXPECT_EQ(mgr.getResource<TestGPUResource>(handle), nullptr); // Should now be null
}

TEST(GPUResourceManagerTest, GetResource_WrongType)
{
    MockGPURenderDevice renderDevice;
    std::unique_ptr<GPUResourceStreamer> streamer = std::make_unique<GPUResourceStreamer>();
    GPUResourceManager mgr(renderDevice, std::move(streamer));

    ResourceHandle handle = 30;
    mgr.storeResource(handle, std::make_unique<TestGPUResource>(123));

    // Try to retrieve a TestGPUResource as an AnotherTestGPUResource
    AnotherTestGPUResource* wrongTypeRes = mgr.getResource<AnotherTestGPUResource>(handle);
    EXPECT_EQ(wrongTypeRes, nullptr); // dynamic_cast should return nullptr

    // Try to retrieve a TestGPUResource as a TestGPUTexture
    TestGPUTexture* wrongDerivedTypeRes = mgr.getResource<TestGPUTexture>(handle);
    EXPECT_EQ(wrongDerivedTypeRes, nullptr); // dynamic_cast should return nullptr
}

TEST(GPUResourceManagerTest, RemoveResource_ValidHandle)
{
    MockGPURenderDevice renderDevice;
    std::unique_ptr<GPUResourceStreamer> streamer = std::make_unique<GPUResourceStreamer>();
    GPUResourceManager mgr(renderDevice, std::move(streamer));

    ResourceHandle handle = 40;
    std::unique_ptr<TestGPUResource> res = std::make_unique<TestGPUResource>(10);
    TestGPUResource* rawRes = res.get(); // Get raw pointer before moving
    mgr.storeResource(handle, std::move(res));

    EXPECT_EQ(TestGPUResource::s_instanceCount, 1); // One instance alive

    mgr.removeResource(handle);

    EXPECT_EQ(mgr.getResource<TestGPUResource>(handle), nullptr); // Should be null now
    EXPECT_EQ(TestGPUResource::s_instanceCount, 0); // Resource should be destroyed
    EXPECT_TRUE(rawRes->m_wasDestroyed); // Verify specific instance destruction
}

TEST(GPUResourceManagerTest, RemoveResource_InvalidHandle)
{
    MockGPURenderDevice renderDevice;
    std::unique_ptr<GPUResourceStreamer> streamer = std::make_unique<GPUResourceStreamer>();
    GPUResourceManager mgr(renderDevice, std::move(streamer));

    ResourceHandle handle1 = 50;
    mgr.storeResource(handle1, std::make_unique<TestGPUResource>(1)); // Add one resource

    // Attempt to remove invalid handles
    mgr.removeResource(999); // Non-existent
    mgr.removeResource(1000); // Another non-existent

    // Verify no crash and the existing resource is still there
    EXPECT_NE(mgr.getResource<TestGPUResource>(handle1), nullptr); // Original resource still there
    EXPECT_EQ(TestGPUResource::s_instanceCount, 1); // No instances should have been destroyed
}

TEST(GPUResourceManagerTest, Destructor_CleansUpResources)
{
    // This test uses a scope to ensure the GPUResourceManager's destructor is called.
    // The static s_instanceCount will verify all TestGPUResource instances are destroyed.
    EXPECT_EQ(TestGPUResource::s_instanceCount, 0); // Should be 0 before test

    {
        MockGPURenderDevice renderDevice;
        std::unique_ptr<GPUResourceStreamer> streamer = std::make_unique<GPUResourceStreamer>();
        GPUResourceManager mgr(renderDevice, std::move(streamer));

        mgr.storeResource(1, std::make_unique<TestGPUResource>(1));
        mgr.storeResource(2, std::make_unique<TestGPUResource>(2));
        mgr.storeResource(3, std::make_unique<TestGPUResource>(3));
        mgr.storeResource(4, std::make_unique<TestGPUTexture>(4)); // Also test derived types
        mgr.storeResource(5, std::make_unique<TestGPUBuffer>(5));   // And another derived type

        EXPECT_EQ(TestGPUResource::s_instanceCount, 3); // Only TestGPUResource instances counted by s_instanceCount
        // TestGPUTexture and TestGPUBuffer are separate types.
        // If we wanted to count all IGPUResource, we'd need a different static counter.
    } // mgr goes out of scope here, triggering its destructor

    EXPECT_EQ(TestGPUResource::s_instanceCount, 0); // All TestGPUResource instances should be destroyed
}

TEST(GPUResourceManagerTest, Destructor_WithRemovedResources)
{
    EXPECT_EQ(TestGPUResource::s_instanceCount, 0);

    {
        MockGPURenderDevice renderDevice;
        std::unique_ptr<GPUResourceStreamer> streamer = std::make_unique<GPUResourceStreamer>();
        GPUResourceManager mgr(renderDevice, std::move(streamer));

        ResourceHandle h1 = 10;
        ResourceHandle h2 = 20;
        ResourceHandle h3 = 30;

        mgr.storeResource(h1, std::make_unique<TestGPUResource>(1));
        mgr.storeResource(h2, std::make_unique<TestGPUResource>(2));
        mgr.storeResource(h3, std::make_unique<TestGPUResource>(3));

        EXPECT_EQ(TestGPUResource::s_instanceCount, 3);

        mgr.removeResource(h1); // Remove one resource
        EXPECT_EQ(TestGPUResource::s_instanceCount, 2); // One should be destroyed

        mgr.removeResource(h3); // Remove another
        EXPECT_EQ(TestGPUResource::s_instanceCount, 1); // Another should be destroyed

        // h2 is still active
    } // mgr goes out of scope, h2 should be destroyed

    EXPECT_EQ(TestGPUResource::s_instanceCount, 0); // All instances should be destroyed
}
