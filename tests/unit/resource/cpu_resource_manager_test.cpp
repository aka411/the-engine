
// cpu_resource_manager_test.cpp
#include "gtest/gtest.h"
#include "cpu_resource_manager.h" // Include your CPUResourceManager header

// Using directives for convenience within the test file
using namespace TheEngine::Resource;
using namespace TheEngine::Core;

// --- Custom Test CPUResource Implementation ---
// This is a simple concrete class derived from CPUResource for testing purposes.
// It allows us to track construction and destruction.
class TestCPUResource : public ICPUResource
{
public:
    int id;
    // Static counter to track how many TestCPUResource objects are currently alive
    static int s_instanceCount;
    // Flag to indicate if this specific instance was destroyed
    bool m_wasDestroyed = false;


	virtual CPUResourceType getType() const override
	{
		return CPUResourceType::UNDEFINED; // Return a default type for testing
	}


    TestCPUResource(int _id) : id(_id)
    {
        s_instanceCount++;
        // std::cout << "TestCPUResource " << id << " constructed. Count: " << s_instanceCount << std::endl;
    }

    ~TestCPUResource() override
    {
        s_instanceCount--;
        m_wasDestroyed = true;
        // std::cout << "TestCPUResource " << id << " destroyed. Count: " << s_instanceCount << std::endl;
    }

    // Prevent copy construction and assignment for unique_ptr compatibility
    TestCPUResource(const TestCPUResource&) = delete;
    TestCPUResource& operator=(const TestCPUResource&) = delete;
};

// Initialize the static member
int TestCPUResource::s_instanceCount = 0;

// --- Another Test CPUResource Type for Type Mismatch Tests ---
class AnotherTestCPUResource : public ICPUResource
{
public:
    int value;
    AnotherTestCPUResource(int val) : value(val) {}

    virtual CPUResourceType getType() const override
    {
        return CPUResourceType::UNDEFINED; // Return a default type for testing
    }
};

// --- CPUResourceManager Test Suite ---
// We use a simple TEST_F fixture if common setup/teardown is needed,
// but for this class, creating a new manager in each TEST is often sufficient
// to ensure complete isolation between tests.
// If you had complex setup (e.g., shared mock objects), a fixture would be better.

TEST(CPUResourceManagerTest, ConstructorInitializesEmpty)
{
    CPUResourceManager mgr;
    // Verify that the internal resource vector is empty
    EXPECT_EQ(mgr.getResourceCount(), 0);
    // Verify that the free index stack is empty
    EXPECT_TRUE(mgr.isFreeIndexStackEmpty());
}

TEST(CPUResourceManagerTest, AddResource_FirstResource)
{
    CPUResourceManager mgr;
    std::unique_ptr<TestCPUResource> res1 = std::make_unique<TestCPUResource>(101);
    TestCPUResource* rawRes1 = res1.get(); // Keep raw pointer to verify content later

    ResourceHandle handle1 = mgr.addResource(std::move(res1));

    EXPECT_EQ(handle1, 0); // First resource should get handle 0
    EXPECT_EQ(mgr.getResourceCount(), 1); // Vector size should be 1
    EXPECT_TRUE(mgr.isFreeIndexStackEmpty()); // No free indices yet

    // Retrieve and verify the resource
    TestCPUResource* retrievedRes = mgr.getResource<TestCPUResource>(handle1);
    ASSERT_NE(retrievedRes, nullptr); // Should not be null
    EXPECT_EQ(retrievedRes->id, 101); // Check content
    EXPECT_EQ(retrievedRes, rawRes1); // Ensure it's the same object
}

TEST(CPUResourceManagerTest, AddResource_MultipleSequential)
{
    CPUResourceManager mgr;
    ResourceHandle h0 = mgr.addResource(std::make_unique<TestCPUResource>(0));
    ResourceHandle h1 = mgr.addResource(std::make_unique<TestCPUResource>(1));
    ResourceHandle h2 = mgr.addResource(std::make_unique<TestCPUResource>(2));

    EXPECT_EQ(h0, 0);
    EXPECT_EQ(h1, 1);
    EXPECT_EQ(h2, 2);
    EXPECT_EQ(mgr.getResourceCount(), 3);
    EXPECT_TRUE(mgr.isFreeIndexStackEmpty());

    EXPECT_EQ(mgr.getResource<TestCPUResource>(h1)->id, 1);
}

TEST(CPUResourceManagerTest, AddResource_ReuseFreeIndex)
{
    CPUResourceManager mgr;
    mgr.addResource(std::make_unique<TestCPUResource>(0)); // h0
    ResourceHandle h1 = mgr.addResource(std::make_unique<TestCPUResource>(1)); // h1
    mgr.addResource(std::make_unique<TestCPUResource>(2)); // h2

    mgr.removeResource(h1); // Free handle 1

    EXPECT_FALSE(mgr.isFreeIndexStackEmpty());
    EXPECT_EQ(mgr.getTopFreeIndex(), h1); // Verify handle 1 is on top of free stack

    std::unique_ptr<TestCPUResource> newRes = std::make_unique<TestCPUResource>(99);
    TestCPUResource* rawNewRes = newRes.get();
    ResourceHandle newHandle = mgr.addResource(std::move(newRes));

    EXPECT_EQ(newHandle, h1); // Expect handle 1 to be reused
    EXPECT_EQ(mgr.getResourceCount(), 3); // Size remains the same
    EXPECT_TRUE(mgr.isFreeIndexStackEmpty()); // Free stack should now be empty

    TestCPUResource* retrievedNew = mgr.getResource<TestCPUResource>(newHandle);
    ASSERT_NE(retrievedNew, nullptr);
    EXPECT_EQ(retrievedNew->id, 99);
    EXPECT_EQ(retrievedNew, rawNewRes); // Ensure it's the same object
}

TEST(CPUResourceManagerTest, GetResource_ValidHandleCorrectType)
{
    CPUResourceManager mgr;
    std::unique_ptr<TestCPUResource> res = std::make_unique<TestCPUResource>(55);
    ResourceHandle handle = mgr.addResource(std::move(res));

    TestCPUResource* retrieved = mgr.getResource<TestCPUResource>(handle);
    ASSERT_NE(retrieved, nullptr);
    EXPECT_EQ(retrieved->id, 55);
}

TEST(CPUResourceManagerTest, GetResource_InvalidHandle)
{
    CPUResourceManager mgr;
    mgr.addResource(std::make_unique<TestCPUResource>(1)); // Add one resource

    // Test out-of-bounds handles
    EXPECT_EQ(mgr.getResource<TestCPUResource>(999), nullptr); // Too large
    EXPECT_EQ(mgr.getResource<TestCPUResource>(-1), nullptr);  // Negative
}

TEST(CPUResourceManagerTest, GetResource_RemovedResource)
{
    CPUResourceManager mgr;
    ResourceHandle handle = mgr.addResource(std::make_unique<TestCPUResource>(1));
    mgr.removeResource(handle); // Remove it

    EXPECT_EQ(mgr.getResource<TestCPUResource>(handle), nullptr); // Should now be null
}

TEST(CPUResourceManagerTest, GetResource_WrongType)
{
    CPUResourceManager mgr;
    ResourceHandle handle = mgr.addResource(std::make_unique<TestCPUResource>(123));

    // Try to retrieve a TestCPUResource as an AnotherTestCPUResource
    AnotherTestCPUResource* wrongTypeRes = mgr.getResource<AnotherTestCPUResource>(handle);
    EXPECT_EQ(wrongTypeRes, nullptr); // dynamic_cast should return nullptr
}

TEST(CPUResourceManagerTest, RemoveResource_ValidHandle)
{
    CPUResourceManager mgr;
    std::unique_ptr<TestCPUResource> res = std::make_unique<TestCPUResource>(10);
    TestCPUResource* rawRes = res.get(); // Get raw pointer before moving
    ResourceHandle handle = mgr.addResource(std::move(res));

    EXPECT_EQ(TestCPUResource::s_instanceCount, 1); // One instance alive

    mgr.removeResource(handle);

    EXPECT_EQ(mgr.getResource<TestCPUResource>(handle), nullptr); // Should be null now
    EXPECT_EQ(TestCPUResource::s_instanceCount, 0); // Resource should be destroyed
    EXPECT_TRUE(rawRes->m_wasDestroyed); // Verify specific instance destruction
    EXPECT_FALSE(mgr.isFreeIndexStackEmpty()); // Free index should have the handle
    EXPECT_EQ(mgr.getTopFreeIndex(), handle);
}

TEST(CPUResourceManagerTest, RemoveResource_InvalidHandle)
{
    CPUResourceManager mgr;
    mgr.addResource(std::make_unique<TestCPUResource>(1)); // Add one resource

    // Attempt to remove invalid handles
    mgr.removeResource(999); // Out of bounds
    mgr.removeResource(-1);  // Negative

    // Verify no crash and state remains unchanged
    EXPECT_EQ(mgr.getResourceCount(), 1);
    EXPECT_NE(mgr.getResource<TestCPUResource>(0), nullptr); // Original resource still there
    EXPECT_TRUE(mgr.isFreeIndexStackEmpty()); // No handles should have been freed
}

TEST(CPUResourceManagerTest, RemoveResource_AlreadyRemoved_BugFixTest)
{
    CPUResourceManager mgr;
    ResourceHandle handle = mgr.addResource(std::make_unique<TestCPUResource>(1));

    mgr.removeResource(handle); // First removal
    EXPECT_FALSE(mgr.isFreeIndexStackEmpty());
    EXPECT_EQ(mgr.getTopFreeIndex(), handle); // Handle is on stack

    mgr.removeResource(handle); // Second removal on the same empty slot

    // The bug fix ensures it's not pushed again.
    // So the stack should still contain only one instance of 'handle'.
    // We can't directly check stack size without more getters, but we can check if it's still not empty
    // and if the top is still the same, implying no new push.
    EXPECT_FALSE(mgr.isFreeIndexStackEmpty());
    EXPECT_EQ(mgr.getTopFreeIndex(), handle); // Still the same handle on top

    // To be more robust, if we added another resource, it should still reuse 'handle' once.
    ResourceHandle newHandle = mgr.addResource(std::make_unique<TestCPUResource>(99));
    EXPECT_EQ(newHandle, handle);
    EXPECT_TRUE(mgr.isFreeIndexStackEmpty()); // Now it should be empty
}

TEST(CPUResourceManagerTest, Destructor_CleansUpResources)
{
    // This test uses a scope to ensure the CPUResourceManager's destructor is called.
    // The static s_instanceCount will verify all TestCPUResource instances are destroyed.
    EXPECT_EQ(TestCPUResource::s_instanceCount, 0); // Should be 0 before test

    {
        CPUResourceManager mgr;
        mgr.addResource(std::make_unique<TestCPUResource>(1));
        mgr.addResource(std::make_unique<TestCPUResource>(2));
        mgr.addResource(std::make_unique<TestCPUResource>(3));

        EXPECT_EQ(TestCPUResource::s_instanceCount, 3); // 3 instances alive
    } // mgr goes out of scope here, triggering its destructor

    EXPECT_EQ(TestCPUResource::s_instanceCount, 0); // All instances should be destroyed
}

TEST(CPUResourceManagerTest, Destructor_WithRemovedResources)
{
    EXPECT_EQ(TestCPUResource::s_instanceCount, 0);

    {
        CPUResourceManager mgr;
        ResourceHandle h1 = mgr.addResource(std::make_unique<TestCPUResource>(1));
        ResourceHandle h2 = mgr.addResource(std::make_unique<TestCPUResource>(2));
        ResourceHandle h3 = mgr.addResource(std::make_unique<TestCPUResource>(3));

        EXPECT_EQ(TestCPUResource::s_instanceCount, 3);

        mgr.removeResource(h1); // Remove one resource
        EXPECT_EQ(TestCPUResource::s_instanceCount, 2); // One should be destroyed

        mgr.removeResource(h3); // Remove another
        EXPECT_EQ(TestCPUResource::s_instanceCount, 1); // Another should be destroyed

        // h2 is still active
    } // mgr goes out of scope, h2 should be destroyed

    EXPECT_EQ(TestCPUResource::s_instanceCount, 0); // All instances should be destroyed
}