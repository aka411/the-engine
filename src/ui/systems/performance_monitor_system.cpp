
#include "ui/utils/geometry_generator.h"
#include "ui/systems/performance_monitor_system.h"
#include "ui/core/ui_component.h"
#include "ui/core/ui_core_system.h"
#include <iostream>



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

	resultMesh.reserve(uiGraphComponentPtr->buffer.size() * (4 * 2) * sizeof(float));

	uiRenderMeshComponent->vertexCount = 0;
	for (float dataPoint : data)
	{
		//100 px = 500 fps
		//if (dataPoint < 0.0001) continue;
		float scaledHeight = (dataPoint/500.0f ) * 100.0f;


		//NOTE : The scaledHeight is negated cause the 
		//geometry generates the rectangles with Y axis up but our UI coordinate system has Y axis down
		GeometryGenerator::MeshData mesh = GeometryGenerator::getColouredRectangleWithOffset(6,-scaledHeight, { offset ,100,0}, uiGraphComponentPtr->color);
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


	//uiRenderMeshComponent->vertexCount =;

	uiRenderMeshComponent->vertexBufferOffset = bufferOffset;
	uiRenderMeshComponent->isDirty = false;



}

void PerformanceMonitorSystem::UpdateMemoryUsage(const size_t UsedBytes)
{



}
