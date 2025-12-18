#include "gtest/gtest.h"

#include "../../../the-engine/ecs/include/ecs_engine.h"
#include "../../../the-engine/ecs/include/common_data_types.h"
#include <algorithm>
#include <string>

//TODO : Write more tests and better ones 

using namespace TheEngine::ECS;

class ECSEngineTest : public testing::Test
{
private:

protected:


    std::unique_ptr<ECSEngine> m_ecsEngine;


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


        m_ecsEngine = std::make_unique<ECSEngine>();

        m_ecsEngine.get()->registerComponent<ComponentA>();
        m_ecsEngine.get()->registerComponent<ComponentB>();

    }

    void TearDown() override
    {
        m_ecsEngine.reset();
    }

};



TEST_F(ECSEngineTest, createStoreRetriveEditGetValidate )
{

    EntityId entityId_1 = m_ecsEngine.get()->createEntity();

    ComponentA componentA;
    componentA.text = "WOW THIS WORKS";

    m_ecsEngine.get()->addComponentToEntity<ComponentA>(entityId_1, componentA);

    m_ecsEngine.get()->processBufferedCommands();

    EntityChunkView entityChunkView_1 = m_ecsEngine.get()->getEntityChunkView(entityId_1);

    ComponentA* retrievedComponentA = entityChunkView_1.getComponent<ComponentA>();

    ASSERT_NE(retrievedComponentA, nullptr);
    ASSERT_EQ(retrievedComponentA->text, "WOW THIS WORKS");




    EntityId entityId_2 = m_ecsEngine.get()->createEntity();

    ComponentA componentA_2;
    componentA_2.text = "WOW THIS WORKS FOR ENTITY 2";

    m_ecsEngine.get()->addComponentToEntity<ComponentA>(entityId_2, componentA_2);

    m_ecsEngine.get()->processBufferedCommands();

    EntityChunkView entityChunkView_2 = m_ecsEngine.get()->getEntityChunkView(entityId_2);

    ComponentA* retrievedComponentA_2 = entityChunkView_2.getComponent<ComponentA>();

    ASSERT_NE(retrievedComponentA_2, nullptr);
    ASSERT_EQ(retrievedComponentA_2->text, "WOW THIS WORKS FOR ENTITY 2");









    ComponentB componentB_1;
    componentB_1.text = "WOW THIS WORKS FOR MOVING ALSO";

    m_ecsEngine.get()->addComponentToEntity<ComponentB>(entityId_1, componentB_1);

    m_ecsEngine.get()->processBufferedCommands();

    EntityChunkView entityChunkViewAgain_1 = m_ecsEngine.get()->getEntityChunkView(entityId_1);

    ComponentB* retrievedComponentB_1 = entityChunkViewAgain_1.getComponent<ComponentB>();

    ASSERT_NE(retrievedComponentB_1, nullptr);
    ASSERT_EQ(retrievedComponentB_1->text, "WOW THIS WORKS FOR MOVING ALSO");

    ComponentA* againRetrievedComponentA_1 = entityChunkViewAgain_1.getComponent<ComponentA>();

    EntityChunkView entityChunkViewAgain_2 = m_ecsEngine.get()->getEntityChunkView(entityId_2);
    
    ASSERT_NE(againRetrievedComponentA_1, nullptr);
    ASSERT_EQ(againRetrievedComponentA_1->text, "WOW THIS WORKS");


  
}




TEST_F(ECSEngineTest, EntityCreation_And_ComponentStorage)
{

    EntityId entityId = m_ecsEngine.get()->createEntity();
    ComponentA initialCompA;
    initialCompA.text = "TEST_DATA_A_1";
    initialCompA.secondsPassed = 5.0f; 


    m_ecsEngine.get()->addComponentToEntity<ComponentA>(entityId, initialCompA);
    m_ecsEngine.get()->processBufferedCommands();

   
    EntityChunkView view = m_ecsEngine.get()->getEntityChunkView(entityId);
    ComponentA* retrievedCompA = view.getComponent<ComponentA>();

  
    ASSERT_NE(retrievedCompA, nullptr) << "ComponentA should have been successfully added.";
    ASSERT_EQ(retrievedCompA->text, "TEST_DATA_A_1");
    ASSERT_EQ(retrievedCompA->secondsPassed, 5.0f);
}


TEST_F(ECSEngineTest, ComponentAddition_TriggersMigration_DataRetained)
{
  
    EntityId entityId = m_ecsEngine.get()->createEntity();
    ComponentA compA_data;
    compA_data.text = "Original A Data";
    m_ecsEngine.get()->addComponentToEntity<ComponentA>(entityId, compA_data);
    m_ecsEngine.get()->processBufferedCommands();


    ComponentB compB_data;
    compB_data.text = "New B Data";
    m_ecsEngine.get()->addComponentToEntity<ComponentB>(entityId, compB_data);
    m_ecsEngine.get()->processBufferedCommands();

 
    EntityChunkView view = m_ecsEngine.get()->getEntityChunkView(entityId);

    ComponentA* compA_after = view.getComponent<ComponentA>();
    ComponentB* compB_after = view.getComponent<ComponentB>();


    ASSERT_NE(compB_after, nullptr) << "ComponentB should be present.";
    ASSERT_EQ(compB_after->text, "New B Data");

 
    ASSERT_NE(compA_after, nullptr) << "ComponentA should still be present after migration.";
    ASSERT_EQ(compA_after->text, "Original A Data") << "ComponentA data should be preserved during migration.";
}





TEST_F(ECSEngineTest, MultipleEntities_AreIsolated)
{
   
    EntityId entityId_1 = m_ecsEngine.get()->createEntity();
    ComponentA compA_1;
    compA_1.text = "E1_A_Data";
    m_ecsEngine.get()->addComponentToEntity<ComponentA>(entityId_1, compA_1);

    EntityId entityId_2 = m_ecsEngine.get()->createEntity();
    ComponentA compA_2;
    compA_2.text = "E2_A_Data";
    m_ecsEngine.get()->addComponentToEntity<ComponentA>(entityId_2, compA_2);

    m_ecsEngine.get()->processBufferedCommands();


    EntityChunkView view_1 = m_ecsEngine.get()->getEntityChunkView(entityId_1);
    ComponentA* retrieved_1 = view_1.getComponent<ComponentA>();
    ASSERT_NE(retrieved_1, nullptr);
    ASSERT_EQ(retrieved_1->text, "E1_A_Data"); 

   
    EntityChunkView view_2 = m_ecsEngine.get()->getEntityChunkView(entityId_2);
    ComponentA* retrieved_2 = view_2.getComponent<ComponentA>();
    ASSERT_NE(retrieved_2, nullptr);
    ASSERT_EQ(retrieved_2->text, "E2_A_Data"); 
}





TEST_F(ECSEngineTest, queryArray)
{
  

    const size_t NUM_OF_ENTITIES = 400;
    std::vector<EntityId> entityIds;
    entityIds.reserve(NUM_OF_ENTITIES);

    
    for (size_t i = 0; i < NUM_OF_ENTITIES; ++i)
    {
        EntityId newEntity = m_ecsEngine->createEntity();
        entityIds.push_back(newEntity);

        // All entities get ComponentA
        ComponentA compA;
        compA.text = "Entity " + std::to_string(i) + " - A";
        m_ecsEngine->addComponentToEntity<ComponentA>(newEntity, compA);

     //only first half entities get component B
        if (i < NUM_OF_ENTITIES / 2)
        {
            ComponentB compB;
            compB.text = "Entity " + std::to_string(i) + " - B";
            m_ecsEngine->addComponentToEntity<ComponentB>(newEntity, compB);
        }
    }


    m_ecsEngine->processBufferedCommands();

    auto queryResult = m_ecsEngine->getQuery<ComponentA, ComponentB>();



    //  Validate the retrieved components
    size_t entityCount = 0;
    const size_t EXPECTED_COUNT = NUM_OF_ENTITIES/2;
    for (auto& chunkArrayView : queryResult.getChunkArrayViews())
    {
        // Get the arrays/pointers for the components in the current chunk
        ComponentA* compAs = chunkArrayView.getComponentArray<ComponentA>();
        ComponentB* compBs = chunkArrayView.getComponentArray<ComponentB>();
        

        size_t entitiesInChunk = chunkArrayView.getCount();

        ASSERT_NE(compAs, nullptr);
        ASSERT_NE(compBs, nullptr);

        for (size_t i = 0; i < entitiesInChunk; ++i)
        {


            // Check ComponentA data
            ASSERT_FALSE(compAs[i].text.empty());
            ASSERT_TRUE(compAs[i].text.find("Entity") != std::string::npos);

            // Check ComponentB data
            ASSERT_FALSE(compBs[i].text.empty());
            ASSERT_TRUE(compBs[i].text.find("Entity") != std::string::npos);

            entityCount++;
        }
    }

    // Final check on the total number of entities iterated
    ASSERT_EQ(entityCount, EXPECTED_COUNT)
        << "The iteration over all chunk views should yield the expected count.";



}


TEST_F(ECSEngineTest, getNonExsistentComponentFromEntity)
{

    EntityId entityId = m_ecsEngine.get()->createEntity();
    ComponentA initialCompA;
    initialCompA.text = "TEST_DATA_A_1";
    initialCompA.secondsPassed = 5.0f;


    m_ecsEngine.get()->addComponentToEntity<ComponentA>(entityId, initialCompA);
    m_ecsEngine.get()->processBufferedCommands();


    EntityChunkView view = m_ecsEngine.get()->getEntityChunkView(entityId);
	ComponentA* retrievedCompA = view.getComponent<ComponentA>();
    ComponentB* retrievedCompB = view.getComponent<ComponentB>();
    
	ASSERT_NE(retrievedCompA, nullptr) << "ComponentA should not be null.";
	ASSERT_EQ(retrievedCompB, nullptr) << "ComponentB should not be present in the entity.";

}






//Archetype Chunk Record Retrival Test
TEST_F(ECSEngineTest, GetChunkRecordArray_ReturnsCorrectEntityIds)
{
    // 1. Setup: Create entities with ComponentA and ComponentB
    const size_t NUM_ENTITIES = 50;
    std::vector<EntityId> expectedIds;
    expectedIds.reserve(NUM_ENTITIES);

    for (size_t i = 0; i < NUM_ENTITIES; ++i)
    {
        EntityId newEntity = m_ecsEngine->createEntity();
        expectedIds.push_back(newEntity);

        ComponentA compA;
        compA.text = "ID_A_" + std::to_string(newEntity.id);
        m_ecsEngine->addComponentToEntity<ComponentA>(newEntity, compA);

        ComponentB compB;
        compB.text = "ID_B_" + std::to_string(newEntity.id);
        m_ecsEngine->addComponentToEntity<ComponentB>(newEntity, compB);
    }

    m_ecsEngine->processBufferedCommands();

    // 2. Query for the created entities
    auto queryResult = m_ecsEngine->getQuery<ComponentA, ComponentB>();

    // 3. Iterate through ChunkArrayViews and validate the record array
    size_t totalEntitiesFound = 0;
    std::vector<EntityId> retrievedIds;

    for (auto& chunkArrayView : queryResult.getChunkArrayViews())
    {
        // Get the pointer to the EntityId array (the function under test)
        const EntityId* entityRecordArray = chunkArrayView.getChunkRecordArray();
        size_t entitiesInChunk = chunkArrayView.getCount();

        // Basic check for non-null and count
        ASSERT_NE(entityRecordArray, nullptr) << "Chunk record array should not be null.";
        ASSERT_GT(entitiesInChunk, 0) << "Chunk should have entities.";

        // Validate individual EntityIds within the chunk
        for (size_t i = 0; i < entitiesInChunk; ++i)
        {
            EntityId currentId = entityRecordArray[i];

       
            // Store the retrieved ID
            retrievedIds.push_back(currentId);

            // Further validation: Ensure the ID is functional by getting the EntityChunkView back
            EntityChunkView view = m_ecsEngine->getEntityChunkView(currentId);
            ASSERT_NE(view.getComponent<ComponentA>(), nullptr)
                << "Should be able to retrieve entity view using the ID from the record array.";

            totalEntitiesFound++;
        }
    }

    // 4. Final validation: Check total count and uniqueness/completeness
    ASSERT_EQ(totalEntitiesFound, NUM_ENTITIES)
        << "The total number of entities retrieved from chunk records must match the total created.";



     auto entityIdSorterMethod = [](const EntityId& a, const EntityId& b)
        {

            if (a.id != b.id)
            {
                return a.id < b.id;
            }

            return a.generation < b.generation;
        };


    // Sort the retrieved IDs and the expected IDs to compare sets, ensuring all IDs are present.
    std::sort(retrievedIds.begin(), retrievedIds.end(), entityIdSorterMethod);

    std::sort(expectedIds.begin(), expectedIds.end(), entityIdSorterMethod);


    ASSERT_EQ(retrievedIds.size(), expectedIds.size());
    for (size_t i = 0; i < expectedIds.size(); ++i)
    {
        ASSERT_EQ(retrievedIds[i].id, expectedIds[i].id) << "Entity IDs in the record array do not match the expected IDs.";
    }
}