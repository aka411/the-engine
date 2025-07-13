// gpu_resource_manager_test.cpp
#include "gtest/gtest.h"
#include "gpu_resource_manager.h"
#include <memory> 

// Forward declarations or includes for IGPUResource and ResourceHandle

namespace TheEngine::Core
{
    // Mock ResourceHandle for testing
    // In a real scenario, this would come from your core/include/types.h
    using ResourceHandle = unsigned int;
}

namespace TheEngine::Graphics
{


    class MockGPUResource : public IGPUResource
    {
    public:
		virtual ResourceType getType() const override
		{
			return ResourceType::UNKNOWN; // Return a default type for testing
		}
    };

   
    class GPUResourceManagerTest : public ::testing::Test
    {
    protected:
        GPUResourceManager manager;

       
        void SetUp() override
        {
            // Code here will be run before each test
        }

        void TearDown() override
        {
            // Code here will be run after each test
        }
    };

    // Test case for the constructor
    TEST_F(GPUResourceManagerTest, ConstructorInitializesEmpty)
    {
    
       
        TheEngine::Core::ResourceHandle handle = 1;
        ASSERT_EQ(nullptr, manager.getResource(handle));
    }

    // Test case for storeResource and getResource - basic functionality
    TEST_F(GPUResourceManagerTest, StoreAndGetResource)
    {
        TheEngine::Core::ResourceHandle handle = 100;
        std::unique_ptr<IGPUResource> resource = std::make_unique<MockGPUResource>();
        IGPUResource* expectedPtr = resource.get(); 

        manager.storeResource(handle, std::move(resource));

        IGPUResource* retrievedResource = manager.getResource(handle);
        ASSERT_NE(nullptr, retrievedResource); 
        ASSERT_EQ(expectedPtr, retrievedResource); 
    }

    // Test case for storeResource - storing a null resource
    TEST_F(GPUResourceManagerTest, StoreNullResourceDoesNothing)
    {
        TheEngine::Core::ResourceHandle handle = 200;
        std::unique_ptr<IGPUResource> nullResource = nullptr;

        manager.storeResource(handle, std::move(nullResource));

        // Attempt to retrieve, should still be nullptr as nothing was stored
        ASSERT_EQ(nullptr, manager.getResource(handle));
    }

    // Test case for storeResource - storing a duplicate resource
    TEST_F(GPUResourceManagerTest, StoreDuplicateResourceDoesNothing)
    {
        TheEngine::Core::ResourceHandle handle = 300;
        std::unique_ptr<IGPUResource> resource1 = std::make_unique<MockGPUResource>();
        IGPUResource* expectedPtr = resource1.get();

        manager.storeResource(handle, std::move(resource1));

        // Try to store another resource with the same handle
        std::unique_ptr<IGPUResource> resource2 = std::make_unique<MockGPUResource>();
        manager.storeResource(handle, std::move(resource2)); // This should do nothing

        // Verify that the original resource is still there
        IGPUResource* retrievedResource = manager.getResource(handle);
        ASSERT_NE(nullptr, retrievedResource);
        ASSERT_EQ(expectedPtr, retrievedResource); // Should still be the first resource
    }

    // Test case for getResource - non-existent resource
    TEST_F(GPUResourceManagerTest, GetNonExistentResourceReturnsNullptr)
    {
        TheEngine::Core::ResourceHandle handle = 400;
        ASSERT_EQ(nullptr, manager.getResource(handle));
    }

    // Test case for removeResource - removing an existing resource
    TEST_F(GPUResourceManagerTest, RemoveExistingResource)
    {
        TheEngine::Core::ResourceHandle handle = 500;
        std::unique_ptr<IGPUResource> resource = std::make_unique<MockGPUResource>();
        manager.storeResource(handle, std::move(resource));

        ASSERT_NE(nullptr, manager.getResource(handle)); // Verify it's there first

        manager.removeResource(handle);

        ASSERT_EQ(nullptr, manager.getResource(handle)); // Should be gone after removal
    }

    // Test case for removeResource - removing a non-existent resource
    TEST_F(GPUResourceManagerTest, RemoveNonExistentResourceDoesNotCrash)
    {
        TheEngine::Core::ResourceHandle handle = 600;
        // Attempt to remove a resource that was never stored
        ASSERT_NO_FATAL_FAILURE(manager.removeResource(handle)); // Should not crash
        ASSERT_EQ(nullptr, manager.getResource(handle)); // Still non-existent
    }

    // Test case for storing multiple resources
    TEST_F(GPUResourceManagerTest, StoreMultipleResources)
    {
        TheEngine::Core::ResourceHandle handle1 = 701;
        TheEngine::Core::ResourceHandle handle2 = 702;
        TheEngine::Core::ResourceHandle handle3 = 703;

        std::unique_ptr<IGPUResource> res1 = std::make_unique<MockGPUResource>();
        std::unique_ptr<IGPUResource> res2 = std::make_unique<MockGPUResource>();
        std::unique_ptr<IGPUResource> res3 = std::make_unique<MockGPUResource>();

        IGPUResource* ptr1 = res1.get();
        IGPUResource* ptr2 = res2.get();
        IGPUResource* ptr3 = res3.get();

        manager.storeResource(handle1, std::move(res1));
        manager.storeResource(handle2, std::move(res2));
        manager.storeResource(handle3, std::move(res3));

        ASSERT_EQ(ptr1, manager.getResource(handle1));
        ASSERT_EQ(ptr2, manager.getResource(handle2));
        ASSERT_EQ(ptr3, manager.getResource(handle3));
    }

    // Test case for removing some resources while others remain
    TEST_F(GPUResourceManagerTest, RemoveSomeResources)
    {
        TheEngine::Core::ResourceHandle handle1 = 801;
        TheEngine::Core::ResourceHandle handle2 = 802;
        TheEngine::Core::ResourceHandle handle3 = 803;

        std::unique_ptr<IGPUResource> res1 = std::make_unique<MockGPUResource>();
        std::unique_ptr<IGPUResource> res2 = std::make_unique<MockGPUResource>();
        std::unique_ptr<IGPUResource> res3 = std::make_unique<MockGPUResource>();

        IGPUResource* ptr1 = res1.get();
        IGPUResource* ptr3 = res3.get();

        manager.storeResource(handle1, std::move(res1));
        manager.storeResource(handle2, std::move(res2));
        manager.storeResource(handle3, std::move(res3));

        manager.removeResource(handle2); // Remove the middle one

        ASSERT_EQ(ptr1, manager.getResource(handle1));      // Should still be there
        ASSERT_EQ(nullptr, manager.getResource(handle2));   // Should be gone
        ASSERT_EQ(ptr3, manager.getResource(handle3));      // Should still be there
    }

    // Test case for getting resource after it's been removed and then re-added
    TEST_F(GPUResourceManagerTest, ReAddResourceAfterRemoval)
    {
        TheEngine::Core::ResourceHandle handle = 900;
        std::unique_ptr<IGPUResource> resource1 = std::make_unique<MockGPUResource>();
        manager.storeResource(handle, std::move(resource1));

        manager.removeResource(handle);
        ASSERT_EQ(nullptr, manager.getResource(handle));

        std::unique_ptr<IGPUResource> resource2 = std::make_unique<MockGPUResource>();
        IGPUResource* expectedPtr = resource2.get();
        manager.storeResource(handle, std::move(resource2));

        ASSERT_EQ(expectedPtr, manager.getResource(handle));
    }

}