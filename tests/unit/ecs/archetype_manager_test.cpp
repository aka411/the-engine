
#include "gtest/gtest.h"

#include <string>
#include "gmock/gmock.h"

#include "../../../the-engine/ecs/include/archetype_manager.h"
#include "../../../the-engine/ecs/include/i_component_registry.h"
#include "../../../the-engine/ecs/include/common_data_types.h"
#include "../../../the-engine/utils/include/logging/null_logger.h"



using namespace TheEngine::ECS;
using namespace TheEngine::Utils;







class MockComponentRegistry : public IComponentRegistry
{
public:
	MOCK_METHOD(ComponentTypeInfo*, getComponentTypeInfo,(ComponentId componentId),(const, override));
};






class ArchetypeManagerTest : public ::testing::Test
{

	private:

		template<typename Component>
		ComponentTypeInfo createComponentTypeInfo()
		{
			ComponentTypeInfo componentTypeInfo;

			componentTypeInfo.size = sizeof(Component);
			componentTypeInfo.alignment = alignof(Component);


			componentTypeInfo.constructor = [](void* mem) { new (mem) Component(); };
			componentTypeInfo.destructor = [](void* mem) { reinterpret_cast<Component*>(mem)->~Component(); };
			componentTypeInfo.copyConstructor = [](void* dest, const void* src)
				{
					new (dest) Component(*reinterpret_cast<const Component*>(src));
				};
			componentTypeInfo.moveConstructor = [](void* dest, void* src)
				{
					new (dest) Component(std::move(*reinterpret_cast<Component*>(src)));
				};

			return componentTypeInfo;
		}



	protected:

		struct ComponentA
		{
			std::string name = "ComponentA";
		};

		struct ComponentB
		{
			std::string name = "ComponentB";
			float componentBFloat = 1.432f;
		};


		struct ComponentC
		{
			std::string name = "ComponentC";
			float componentBFloat = 1.432f;
			int integer = 23;
			std::vector<int> stlVector { 3,4,5 };
		};

		ComponentId m_componentIdA = 1;
		ComponentTypeInfo m_componentTypeInfoA = createComponentTypeInfo<ComponentA>();

		ComponentId m_componentIdB = 2;
		ComponentTypeInfo m_componentTypeInfoB = createComponentTypeInfo<ComponentB>();

		ComponentId m_componentIdC = 3;
		ComponentTypeInfo m_componentTypeInfoC = createComponentTypeInfo<ComponentC>();

		EntityId m_entityId1;
		EntityId m_entityId2;
		EntityId m_entityId3;




		EntityRecord m_entityRecord1;
		EntityRecord m_entityRecord2;
		EntityRecord m_entityRecord3;

		NullLogger m_logger;
		MockComponentRegistry m_mockComponentRegistry;



		
		ArchetypeManager* m_archetypeManager;

	void SetUp() override
	{

		m_entityId1.generation = 0;
		m_entityId1.id = 1;
	
		m_entityId2.generation = 0;
		m_entityId2.id = 2;

		m_entityId3.generation = 0;
		m_entityId3.id = 3;


		m_entityRecord1.archetypeChunkHeader = nullptr;
		m_entityRecord1.archetypeSignature = 0;
		m_entityRecord1.entityId = m_entityId1;
		m_entityRecord1.indexInArchetypeChunkRecordList = 0;

		m_entityRecord2.archetypeChunkHeader = nullptr;
		m_entityRecord2.archetypeSignature = 0;
		m_entityRecord2.entityId = m_entityId2;
		m_entityRecord2.indexInArchetypeChunkRecordList = 0;




	    m_archetypeManager = new ArchetypeManager(m_logger, m_mockComponentRegistry);







	}





	void TearDown() override
	{

		delete m_archetypeManager;


	}





};


TEST_F(ArchetypeManagerTest, InitialAddSingleEntityWithComponentsToArchetypeChunk)
{




	EXPECT_CALL(m_mockComponentRegistry, getComponentTypeInfo(m_componentIdA)).WillOnce(::testing::Return(&m_componentTypeInfoA));

	EXPECT_CALL(m_mockComponentRegistry, getComponentTypeInfo(m_componentIdC)).WillOnce(::testing::Return(&m_componentTypeInfoC));


	ComponentA componentA{};
	ComponentC componentC{};
	
	EntityAddInfo entity1AddInfo;
	entity1AddInfo.entityRecord = m_entityRecord1;
	entity1AddInfo.newArchetypeSignature.set(m_componentIdA);
	entity1AddInfo.newArchetypeSignature.set(m_componentIdC);
/*
	struct ComponentData
	{

		ComponentId componentId;

		void* ptr; //pointer to component data in contiguous memory block

	};
*/

	entity1AddInfo.componentDataList.push_back({ m_componentIdA ,&componentA });
	entity1AddInfo.componentDataList.push_back({ m_componentIdC ,&componentC });



	std::vector<EntityRecordUpdate> entityRecordUpdates = m_archetypeManager->addComponentToEntity(entity1AddInfo);


	ASSERT_EQ(entityRecordUpdates.size(), 1);

	const EntityRecordUpdate& entityRecordUpdate1 = entityRecordUpdates[0];

	ASSERT_NE(entityRecordUpdate1.newArchetypeChunkHeader, nullptr);

	ASSERT_EQ(entityRecordUpdate1.entityId.id, m_entityRecord1.entityId.id);
	ASSERT_EQ(entityRecordUpdate1.entityId.generation, m_entityRecord1.entityId.generation);

	ASSERT_EQ(entityRecordUpdate1.newIndexInArchetypeChunkRecordList, 0);//it should be one cause its the only one we added


	




}

TEST_F(ArchetypeManagerTest, SecondEntityWithSameSignatureUsesExistingArchetype)
{


	EXPECT_CALL(m_mockComponentRegistry, getComponentTypeInfo(m_componentIdA)).WillOnce(::testing::Return(&m_componentTypeInfoA));

	EXPECT_CALL(m_mockComponentRegistry, getComponentTypeInfo(m_componentIdC)).WillOnce(::testing::Return(&m_componentTypeInfoC));


	ComponentA componentA[2];
	ComponentC componentC[2];

	EntityAddInfo entity1AddInfo;
	entity1AddInfo.entityRecord = m_entityRecord1;
	entity1AddInfo.newArchetypeSignature.set(m_componentIdA);
	entity1AddInfo.newArchetypeSignature.set(m_componentIdC);

	EntityAddInfo entity2AddInfo;
	entity2AddInfo.entityRecord = m_entityRecord2;
	entity2AddInfo.newArchetypeSignature.set(m_componentIdA);
	entity2AddInfo.newArchetypeSignature.set(m_componentIdC);

	entity1AddInfo.componentDataList.push_back({ m_componentIdA ,&componentA[0]});
	entity1AddInfo.componentDataList.push_back({ m_componentIdC ,&componentC[0]});

	entity2AddInfo.componentDataList.push_back({ m_componentIdA ,&componentA[1] });
	entity2AddInfo.componentDataList.push_back({ m_componentIdC ,&componentC[1] });



	std::vector<EntityRecordUpdate> entity1RecordUpdates = m_archetypeManager->addComponentToEntity(entity1AddInfo);
	std::vector<EntityRecordUpdate> entity2RecordUpdates = m_archetypeManager->addComponentToEntity(entity2AddInfo);

	ASSERT_EQ(entity1RecordUpdates.size(), 1);
	ASSERT_EQ(entity2RecordUpdates.size(), 1);

	const EntityRecordUpdate& entityRecordUpdate1 = entity1RecordUpdates[0];
	const EntityRecordUpdate& entityRecordUpdate2 = entity2RecordUpdates[0];

	ASSERT_NE(entityRecordUpdate1.newArchetypeChunkHeader, nullptr);
	ASSERT_EQ(entityRecordUpdate1.entityId.id, m_entityRecord1.entityId.id);
	ASSERT_EQ(entityRecordUpdate1.entityId.generation, m_entityRecord1.entityId.generation);
	ASSERT_LT(entityRecordUpdate1.newIndexInArchetypeChunkRecordList, 2);


	ASSERT_NE(entityRecordUpdate2.newArchetypeChunkHeader, nullptr);
	ASSERT_EQ(entityRecordUpdate2.entityId.id, m_entityRecord2.entityId.id);
	ASSERT_EQ(entityRecordUpdate2.entityId.generation, m_entityRecord2.entityId.generation);
	ASSERT_LT(entityRecordUpdate2.newIndexInArchetypeChunkRecordList, 2);


	ASSERT_NE(entityRecordUpdate1.newArchetypeChunkHeader->archetypeDefinition, nullptr);
	ASSERT_NE(entityRecordUpdate2.newArchetypeChunkHeader->archetypeDefinition, nullptr);

	ASSERT_EQ(entityRecordUpdate1.newArchetypeChunkHeader->archetypeDefinition->archetypeSignature, entityRecordUpdate2.newArchetypeChunkHeader->archetypeDefinition->archetypeSignature);
	ASSERT_EQ(entityRecordUpdate1.newArchetypeChunkHeader, entityRecordUpdate2.newArchetypeChunkHeader);



}