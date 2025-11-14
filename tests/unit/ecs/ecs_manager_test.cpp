#include "gtest/gtest.h"

#include "../../../the-engine/ecs/include/ecs_manager.h"
#include "../../../the-engine/ecs/include/common_data_types.h"

//Feels like this is more of a integration test
using namespace TheEngine::ECS;

class ECSManagerTest : public testing::Test
{
private:

protected:

    TheEngine::Utils::NullLogger m_logger;
    std::optional<ComponentRegistry> m_componentRegistry;
    std::optional<EntityManager> m_entityManager;
    std::unique_ptr<ArchetypeManager> m_archetypeManager;

    std::unique_ptr<ECSManager> m_ecsManager;


public:

    struct ComponentA
    {
        std::string text = "Hello";
        float secondsPassed = 10.567;
    };

    struct ComponentB
    {
        std::string text = "World";
        float secondsPassed = 10.567;
        std::vector<int> dummyVector{ 1,3,5,7 };
    };



    void SetUp() override
    {
       

       // m_ecsManager = std::make_unique<ECSManager>(m_componentRegistry, m_entityManager, m_archetypeManager);


        
        m_componentRegistry.emplace();
        m_entityManager.emplace();

      
        m_archetypeManager = std::make_unique<ArchetypeManager>(
            m_logger,
            *m_componentRegistry
        );

       
        m_ecsManager = std::make_unique<ECSManager>(
            *m_componentRegistry,
            *m_entityManager,
            *m_archetypeManager 
        );
    }

    void TearDown() override
    {
        m_ecsManager.reset();
    }

};



TEST_F(ECSManagerTest, CreateEntity)
{

    EntityId entityId_1 = m_ecsManager.get()->createEntity();
    EntityId entityId_2 = m_ecsManager.get()->createEntity();

    //brittle
    ASSERT_EQ(entityId_1.id, 0);
    ASSERT_EQ(entityId_1.generation, 0);
    ASSERT_EQ(entityId_2.id, 1);
    ASSERT_EQ(entityId_2.generation, 0);

}


TEST_F(ECSManagerTest, RegisterComponent)
{
    ComponentId componentAId = m_ecsManager.get()->registerComponent<ComponentA>();
    ComponentId componentBId = m_ecsManager.get()->registerComponent<ComponentB>();

    ASSERT_NE(componentAId, componentBId);



    ComponentId duplicateComponentBId = m_ecsManager.get()->registerComponent<ComponentB>();
    ASSERT_EQ(componentBId, duplicateComponentBId);


    ComponentId duplicateComponentAId = m_ecsManager.get()->registerComponent<ComponentA>();
    ASSERT_EQ(componentAId, duplicateComponentAId);


}



