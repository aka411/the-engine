#include <cstdint>
#include <cassert>

#include "ui/text/ui_text_system.h"
#include "components.h"
#include "ui/core/ui_component.h"

namespace TheEngine::UI
{

	UITextSystem::UITextSystem(UICoreSystem& uiCoreSystem) :
		m_uiCoreSystem(uiCoreSystem)
	{

	}



	//offset is in bytes
	size_t UITextSystem::uploadTextVertexData(std::byte* data, const size_t size, const UIBufferType uiBufferType)
	{

		return m_uiCoreSystem.getUIVertexBufferManagementSystem().uploadTextVertexData(
			UIVertexFormat::TEXT_UI_VERTEX_FORMAT,
			uiBufferType,
			data,
			size
		);


	}



	






	void UITextSystem::updateText()
	{
		//generate dirty text etc
		ECS::ECSEngine& ecsEngine = m_uiCoreSystem.getECSEngine();

		ECS::Query textQuery = ecsEngine.createQuery()
			.with<EngineTransformationComponent, UITextComponent, UITextMeshComponent, UIRectDimensionsComponent>()
			.build();
		
		UITextGenerationSystem uiTextGenerationSystem;

		for (auto& chunk : textQuery.getChunkArrayViews())
		{

			const size_t count = chunk.getCount();

			EngineTransformationComponent* engineTransformationComponentArray = chunk.getComponentArray<EngineTransformationComponent>();
			UITextComponent* uiTextComponentArray = chunk.getComponentArray<UITextComponent>();

			UITextMeshComponent* uiTextMeshComponentArray = chunk.getComponentArray<UITextMeshComponent>();
			UIRectDimensionsComponent* uiRectDimensionsComponentArray = chunk.getComponentArray<UIRectDimensionsComponent>();


			if (engineTransformationComponentArray == nullptr ||
				uiTextComponentArray == nullptr ||
				uiTextMeshComponentArray == nullptr ||
				uiRectDimensionsComponentArray == nullptr) continue;



			for (size_t i = 0; i < count; ++i)
			{

				if (!uiTextComponentArray[i].isDirty) continue;
				std::string fontName = uiTextComponentArray[i].fontName;
				//if(m_uiFontManager.getFontAsset(fontName) == nullptr)
				//{
				//	assert(false && "Default font not loaded in UITextSystem");
				//	continue;
				//}

				std::vector<TextVertexData> textVertexData	= uiTextGenerationSystem.generateTextVertexData(
					uiTextComponentArray[i].getString(),
					{ uiRectDimensionsComponentArray[i].width,uiRectDimensionsComponentArray[i].height},
					uiTextComponentArray[i].fontSize,
					{ uiTextComponentArray[i].color.r, uiTextComponentArray[i].color.g, uiTextComponentArray[i].color.b },
					m_uiFontManager.getFontAsset(fontName)->fontData
				);


				const size_t vertexCount = textVertexData.size();
			
				const size_t offsetInBuffer = uploadTextVertexData(
					reinterpret_cast<std::byte*>(textVertexData.data()),
					sizeof(TextVertexData) * textVertexData.size(),
					uiTextComponentArray[i].bufferType
				);



				uiTextComponentArray[i].isDirty = false;
				uiTextMeshComponentArray[i].vertexBufferOffset = offsetInBuffer;
				uiTextMeshComponentArray[i].vertexCount = vertexCount;




			}

		}






	}


	void UITextSystem::runTextSystem()
	{


		updateText();

	}


	UIFontManager& UITextSystem::getUIFontManager()
	{
		return m_uiFontManager;
	}









}