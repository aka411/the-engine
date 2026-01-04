#include "ui/systems/ui_mesh_system.h"
#include "ui/utils/geometry_generator.h"
#include "ui/core/ui_component.h"



namespace TheEngine::UI
{



	UIMeshSystem::UIMeshSystem(UICoreSystem& uiCoreSystem)
		: m_uiCoreSystem(uiCoreSystem), m_ecsEngine(uiCoreSystem.getECSEngine())
	{


	}



	//TODO : Need to change this
	void UIMeshSystem::generateUIMeshesForDirty()
	{
		ECS::Query uiMeshQuery = m_ecsEngine.getQuery<UIRenderMeshComponent, UIRectDimensionsComponent, UIMaterialComponent>();



		for (auto& chunkArrayView : uiMeshQuery.getChunkArrayViews())
		{
			const size_t count = chunkArrayView.getCount();


			UIRectDimensionsComponent* uiRectDimensionsComponentArray = chunkArrayView.getComponentArray<UIRectDimensionsComponent>();
			UIRenderMeshComponent* uRenderMeshComponentArray = chunkArrayView.getComponentArray<UIRenderMeshComponent>();
			UIMaterialComponent* uiMaterialComponentArray = chunkArrayView.getComponentArray<UIMaterialComponent>();

			//use the geometry generator to create a quad mesh based on rect bounds

			if (uiRectDimensionsComponentArray == nullptr || uRenderMeshComponentArray == nullptr)
			{
				continue;
			}

			for (size_t i = 0; i < count; ++i)
			{
				if (!uRenderMeshComponentArray[i].isDirty)
				{
					continue;
				}
				const float width = uiRectDimensionsComponentArray[i].width;
				const float height = uiRectDimensionsComponentArray[i].height;



				GeometryGenerator::MeshData quadMeshData = GeometryGenerator::getColouredRectangle(width, height, uiMaterialComponentArray[i].color);

				const auto bufferType = uRenderMeshComponentArray[i].bufferType;
				const auto vertexFormat = uRenderMeshComponentArray[i].vertexFormat;

				size_t offset = m_uiCoreSystem.getUIVertexBufferManagementSystem().uploadVertexData(vertexFormat, bufferType, quadMeshData.data.data(), quadMeshData.data.size());

				uRenderMeshComponentArray[i].vertexBufferOffset = offset;
				uRenderMeshComponentArray[i].vertexCount = quadMeshData.numOfVertex/*Not in Bytes*/;
				uRenderMeshComponentArray[i].isDirty = false;

			}
		}






	}










}