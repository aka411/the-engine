#include "gtest/gtest.h"
#include "../../../the-engine/ecs/include/common_data_types.h"
#include "../../../the-engine/ecs/include/entity_manager.h"


using namespace TheEngine::ECS;


class EntityManagerTest : public ::testing::Test
{

protected:
    
    std::unique_ptr<EntityManager> m_entityManager;


    void SetUp() override
    {
        
        m_entityManager = std::make_unique<EntityManager>();



    }

    void TearDown() override
    {
        // Code here will be run after each test
        m_entityManager.reset();
    }

};



TEST_F(EntityManagerTest, CreateSignleEntity)
{


    EntityId entityId = m_entityManager.get()->createEntity();

    ASSERT_EQ(entityId.id, 0);
    ASSERT_EQ(entityId.generation, 0);


}

TEST_F(EntityManagerTest, CreateTwoEntities)
{


    EntityId entityId_1 = m_entityManager.get()->createEntity();

    ASSERT_EQ(entityId_1.id, 0);
    ASSERT_EQ(entityId_1.generation, 0);


    EntityId entityId_2 = m_entityManager.get()->createEntity();

    ASSERT_EQ(entityId_2.id, 1);
    ASSERT_EQ(entityId_2.generation, 0);


}

TEST_F(EntityManagerTest, FreedEntityReuse)
{
    //This test is a little brittle

    EntityId entityId_1 = m_entityManager.get()->createEntity();

    ASSERT_EQ(entityId_1.id, 0);
    ASSERT_EQ(entityId_1.generation, 0);


    EntityId entityId_2 = m_entityManager.get()->createEntity();

    ASSERT_EQ(entityId_2.id, 1);
    ASSERT_EQ(entityId_2.generation, 0);

    m_entityManager.get()->destroyEntity(entityId_1);


    EntityId entityId_3 = m_entityManager.get()->createEntity();

    ASSERT_EQ(entityId_3.id, 0);
    ASSERT_EQ(entityId_3.generation, 1);

}