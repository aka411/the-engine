
#include "ui/systems/performance_monitor_system.h"
#include "ui/core/ui_component.h"
#include "ui/core/ui_core_system.h"
#include <ui/utils/ui_geometry_generator.h>



PerformanceMonitorSystem::PerformanceMonitorSystem(TheEngine::UI::UICoreSystem& uiCoreSystem) :m_uiCoreSystem(uiCoreSystem), m_ecsEngine(uiCoreSystem.getECSEngine())
{

	m_ecsEngine.registerComponent<TheEngine::UI::UIGraphComponent>();



}

void PerformanceMonitorSystem::setUp(TheEngine::UI::UIElement&& fpsGraphElement)
{
	
	m_fpsGraphElement = std::make_unique<TheEngine::UI::UIElement>(std::move(fpsGraphElement));


}

void PerformanceMonitorSystem::UpdateFPSMeter(const float deltaTime)
{

	assert(m_fpsGraphElement != nullptr);

	ECS::EntityChunkView entityChunkView = m_ecsEngine.getEntityChunkView(m_fpsGraphElement->getEntityId());

	TheEngine::UI::UIGraphComponent* uiGraphComponentPtr = entityChunkView.getComponent<TheEngine::UI::UIGraphComponent>();
	TheEngine::UI::UIRenderMeshComponent* uiRenderMeshComponent = entityChunkView.getComponent<TheEngine::UI::UIRenderMeshComponent>();
	TheEngine::UI::UIRectDimensionsComponent* uiRectDimensionsComponentPtr = entityChunkView.getComponent<TheEngine::UI::UIRectDimensionsComponent>();
	


	uiGraphComponentPtr->store(1.0f / (deltaTime + 0.0000001));//frames per second

	//TODO : need more optimization
	//generate mesh
	const float maxValue = uiGraphComponentPtr->maxValue;
	std::vector<float> data = uiGraphComponentPtr->getData();

	float offset = 0;//x axis

	std::vector<std::byte> resultMesh;
	resultMesh.reserve(uiGraphComponentPtr->buffer.size() * (4 * 2) * sizeof(float));//bad ,allocation per frame
	//i know its bad but i am just getting stuff tested and seeing it all work, premature optimization is the root of all evil

	/*
	uiRenderMeshComponent->vertexCount = 0;
	for (float dataPoint : data)
	{
		//100 px = 500 fps
		//if (dataPoint < 0.0001) continue;
		float scaledHeight = (dataPoint/500.0f ) * 100.0f;

		if (scaledHeight > uiRectDimensionsComponentPtr->height)
		{
			//cap it to the max height of the graph rect
			scaledHeight = uiRectDimensionsComponentPtr->height-1;
		}

		//NOTE : The scaledHeight is negated cause the 
		//geometry generates the rectangles with Y axis up but our UI coordinate system has Y axis down
	
		UIGeometryGenerator::MeshData mesh = UIGeometryGenerator::generateRectangle(6,-scaledHeight, { offset ,100,0}, uiGraphComponentPtr->color);

		offset += 6;

		if (uiRectDimensionsComponentPtr->width < offset) break;
		uiRenderMeshComponent->vertexCount += mesh.numOfVertex;

		resultMesh.insert(
			resultMesh.end(),
			mesh.data.begin(),
			mesh.data.end()
		);
	}
	const auto vertexFormat = uiRenderMeshComponent->vertexFormat;
	const auto bufferType = uiRenderMeshComponent->bufferType;

	

	size_t bufferOffset = m_uiCoreSystem.getUIVertexBufferManagementSystem().uploadVertexData(vertexFormat, bufferType, resultMesh.data(), resultMesh.size());


	

	uiRenderMeshComponent->vertexBufferOffset = bufferOffset;
	uiRenderMeshComponent->isDirty = false;


	*/


	uiRenderMeshComponent->vertexCount = 0;
	for (int i = 0 ; i < data.size()-1 ; ++i)
	{
		//100 px = 500 fps
	//Note : TO fix aliasing we can interpolate and create more smooth data points between two points
		float scaledHeightA = (data[i] / 500.0f) * 100.0f;
		float scaledHeightB =  (data[i + 1] / 500.0f) * 100.0f;
	


		if (scaledHeightA > uiRectDimensionsComponentPtr->height)
		{
			//cap it to the max height of the graph rect
			scaledHeightA = uiRectDimensionsComponentPtr->height - 1;
		}
		if (scaledHeightB > uiRectDimensionsComponentPtr->height)
		{
			//cap it to the max height of the graph rect
			scaledHeightB = uiRectDimensionsComponentPtr->height - 1;
		}

		//NOTE : UI coordinate system is Y down
		
		const float width = uiRectDimensionsComponentPtr->width / (data.size()/2.0f);
		UIGeometryGenerator::MeshData mesh = UIGeometryGenerator::generateShadedLineGraph(scaledHeightA, scaledHeightB , width, { offset ,uiRectDimensionsComponentPtr->height,0 }, { 0.176f, 0.608f, 0.941f, 1.0f }/*uiGraphComponentPtr->color*/, { 0.176f, 0.608f, 0.941f, 0.3f },false);
		//12, 13
		offset += width;

		if (uiRectDimensionsComponentPtr->width < offset) break;

		uiRenderMeshComponent->vertexCount += mesh.numOfVertex;

		resultMesh.insert(
			resultMesh.end(),
			mesh.data.begin(),
			mesh.data.end()
		);
	}

	const auto vertexFormat = uiRenderMeshComponent->vertexFormat;
	const auto bufferType = uiRenderMeshComponent->bufferType;



	size_t bufferOffset = m_uiCoreSystem.getUIVertexBufferManagementSystem().uploadVertexData(vertexFormat, bufferType, resultMesh.data(), resultMesh.size());




	uiRenderMeshComponent->vertexBufferOffset = bufferOffset;
	uiRenderMeshComponent->isDirty = false;


	
}

void PerformanceMonitorSystem::UpdateMemoryUsage(const size_t UsedBytes)
{



}
