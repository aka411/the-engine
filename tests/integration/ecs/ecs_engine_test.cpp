#include "gtest/gtest.h"

#include "../../../the-engine/ecs/include/ecs_engine.h"
#include "../../../the-engine/ecs/include/common_data_types.h"

//Feels like this is more of a integration test
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