#include "gtest/gtest.h"
#include "gmock/gmock.h"


#include "gpu_resource_system.h" 

// Include headers for the interfaces being mocked
#include "igpu_resource_manager.h"
#include "icpu_resource_manager.h"
#include "igpu_resource_factory.h"
#include "igpu_resource.h" 
#include "icpu_resource.h" 





// --- Mock Implementations of Dependencies ---

class MockGPUResource : public TheEngine::Graphics::IGPUResource
{
public:
    MOCK_METHOD(TheEngine::Graphics::ResourceType, getType, (), (const, override));

};

// A simple mock for ICPUResource 
class MockCPUResource : public TheEngine::Resource::ICPUResource
{
public:
    MOCK_METHOD(TheEngine::Resource::CPUResourceType, getType, (), (const, override));
};


// Mock IGPUResourceManager
class MockGPUResourceManager : public TheEngine::Graphics::IGPUResourceManager
{
public:
    MOCK_METHOD(TheEngine::Graphics::IGPUResource*, getResource, (const TheEngine::Core::ResourceHandle handle), (const, override));

    MOCK_METHOD(void, storeResource, (const TheEngine::Core::ResourceHandle handle, std::unique_ptr<TheEngine::Graphics::IGPUResource>&& resource), (override));
};

// Mock ICPUResourceManager
class MockCPUResourceManager : public TheEngine::Resource::ICPUResourceManager
{
public:
    MOCK_METHOD(TheEngine::Resource::ICPUResource*, getCPUResource, (const TheEngine::Core::ResourceHandle handle), (const, override));
};

// Mock IGPUResourceFactory
class MockGPUResourceFactory : public TheEngine::Graphics::IGPUResourceFactory
{
public:
    MOCK_METHOD(std::unique_ptr<TheEngine::Graphics::IGPUResource>, createGPUResource, (const TheEngine::Resource::ICPUResource& cpuResource), (override));
};


// --- Test Fixture ---
// A test fixture helps set up common objects for multiple tests.
class GPUResourceSystemTest : public ::testing::Test
{
protected:
    // Declare mock objects
    MockGPUResourceManager mockGpuResourceManager;
    MockCPUResourceManager mockCpuResourceManager;
    MockGPUResourceFactory mockGpuResourceFactory;

    // The system under test (SUT)
    TheEngine::Graphics::GPUResourceSystem* sut;

    // Test setup (called before each test)
    void SetUp() override
    {

        sut = new TheEngine::Graphics::GPUResourceSystem(mockGpuResourceManager, mockCpuResourceManager, mockGpuResourceFactory);
    }

    // Test teardown (called after each test)
    void TearDown() override
    {
        delete sut;
    }

    // Helper for resource handles
    TheEngine::Core::ResourceHandle testHandle = TheEngine::Core::ResourceHandle(123);
    TheEngine::Core::ResourceHandle anotherHandle = TheEngine::Core::ResourceHandle(456);
};


// --- Test Cases ---

// Test Case 1: GPU Resource is already present (cache hit)
TEST_F(GPUResourceSystemTest, GetGPUResource_CacheHit)
{
    MockGPUResource* existingGpuResource = new MockGPUResource(); 


    EXPECT_CALL(mockGpuResourceManager, getResource(testHandle))
        .WillOnce(::testing::Return(existingGpuResource)); // Return raw pointer for initial check

    // Expect no calls to CPU manager or factory if GPU resource is found
    EXPECT_CALL(mockCpuResourceManager, getCPUResource(::testing::_)).Times(0);
    EXPECT_CALL(mockGpuResourceFactory, createGPUResource(::testing::_)).Times(0);
    // Expect no storeResource call
    EXPECT_CALL(mockGpuResourceManager, storeResource(::testing::_, ::testing::A<std::unique_ptr<TheEngine::Graphics::IGPUResource>&&>())).Times(0); 

    // Call the method under test
    TheEngine::Graphics::IGPUResource* result = sut->getGPUResource(testHandle);

    // Assertions
    ASSERT_NE(result, nullptr);
    ASSERT_EQ(result, existingGpuResource); // Should return the same existing resource
}


// Test Case 2: GPU Resource not found, CPU Resource found, GPU Creation Success
TEST_F(GPUResourceSystemTest, GetGPUResource_CacheMiss_CPUSuccess_GPUCreateSuccess)
{
	::testing::InSequence s; // Important line to ensure the order of expectations
    // 1. Initial GPU lookup is a miss
    EXPECT_CALL(mockGpuResourceManager, getResource(testHandle))
        .WillOnce(::testing::Return(nullptr)); // First call: GPU cache miss

    // 2. CPU resource is found
    MockCPUResource* cpuResource = new MockCPUResource(); // Owned by expectation
    EXPECT_CALL(mockCpuResourceManager, getCPUResource(testHandle))
        .WillOnce(::testing::Return(cpuResource));

    // 3. GPU resource creation is successful
    auto newGpuResourceUniquePtr = std::make_unique<MockGPUResource>();
    MockGPUResource* newGpuResourceRawPtr = newGpuResourceUniquePtr.get(); // Get raw ptr before move
    EXPECT_CALL(mockGpuResourceFactory, createGPUResource(::testing::Ref(*cpuResource))) // Expect call with specific CPU resource
        .WillOnce(::testing::Return(::testing::ByMove(std::move(newGpuResourceUniquePtr)))); // Factory returns unique_ptr

    // 4. New GPU resource is added to manager

    EXPECT_CALL(mockGpuResourceManager, storeResource(testHandle, ::testing::A<std::unique_ptr<TheEngine::Graphics::IGPUResource>&&>()))
        .WillOnce(::testing::Return()); // Expect storeResource to be called

    // 5. Final getResource call to the manager to retrieve the newly added resource
    EXPECT_CALL(mockGpuResourceManager, getResource(testHandle))
        .WillOnce(::testing::Return(newGpuResourceRawPtr)); // Return the now-managed resource

    // Call the method under test
    TheEngine::Graphics::IGPUResource* result = sut->getGPUResource(testHandle);

    // Assertions
    ASSERT_NE(result, nullptr);
    ASSERT_EQ(result, newGpuResourceRawPtr); // Should return the newly created and added resource
}


// Test Case 3: GPU Resource not found, CPU Resource found, GPU Creation Fails
TEST_F(GPUResourceSystemTest, GetGPUResource_CacheMiss_CPUSuccess_GPUCreateFails)
{
    // 1. Initial GPU lookup is a miss
    EXPECT_CALL(mockGpuResourceManager, getResource(testHandle))
        .WillOnce(::testing::Return(nullptr));

    // 2. CPU resource is found
    MockCPUResource* cpuResource = new MockCPUResource(); // Owned by expectation
    EXPECT_CALL(mockCpuResourceManager, getCPUResource(testHandle))
        .WillOnce(::testing::Return(cpuResource));

    // 3. GPU resource creation fails (returns nullptr)
    EXPECT_CALL(mockGpuResourceFactory, createGPUResource(::testing::Ref(*cpuResource)))
        .WillOnce(::testing::Return(nullptr)); // Factory returns nullptr

    // Expect no storeResource call if creation failed
    EXPECT_CALL(mockGpuResourceManager, storeResource(::testing::_, ::testing::A<std::unique_ptr<TheEngine::Graphics::IGPUResource>&&>())).Times(0); 

    // Call the method under test
    TheEngine::Graphics::IGPUResource* result = sut->getGPUResource(testHandle);

    // Assertions
    ASSERT_EQ(result, nullptr); // Should return nullptr
}

// Test Case 4: GPU Resource not found, CPU Resource not found
TEST_F(GPUResourceSystemTest, GetGPUResource_CacheMiss_CPUResourceNotFound)
{
    // 1. Initial GPU lookup is a miss
    EXPECT_CALL(mockGpuResourceManager, getResource(testHandle))
        .WillOnce(::testing::Return(nullptr));

    // 2. CPU resource is NOT found
    EXPECT_CALL(mockCpuResourceManager, getCPUResource(testHandle))
        .WillOnce(::testing::Return(nullptr));

    // Expect no calls to GPU factory or storeResource
    EXPECT_CALL(mockGpuResourceFactory, createGPUResource(::testing::_)).Times(0);
    EXPECT_CALL(mockGpuResourceManager, storeResource(::testing::_, ::testing::A<std::unique_ptr<TheEngine::Graphics::IGPUResource>&&>())).Times(0);

    // Call the method under test
    TheEngine::Graphics::IGPUResource* result = sut->getGPUResource(testHandle);

    // Assertions
    ASSERT_EQ(result, nullptr); // Should return nullptr
}